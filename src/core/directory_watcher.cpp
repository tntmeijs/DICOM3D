#include "directory_watcher.hpp"

// Spdlog
#include "spdlog/spdlog.h"

// C++ standard
#include <cstdint>

dcm::DCMDirectoryWatcher::DCMDirectoryWatcher(std::string_view directory, std::uint32_t scan_interval) :
	m_is_listening(true),
	m_directory_to_monitor(directory),
	m_scan_interval(scan_interval)
{
}

dcm::DCMDirectoryWatcher::~DCMDirectoryWatcher()
{
	StopWatching();
}

void dcm::DCMDirectoryWatcher::StartWatching()
{
	auto monitoring_loop = [&]()
	{
		while (m_is_listening)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(m_scan_interval));
			RefreshDirectoryTree(std::filesystem::path(m_directory_to_monitor));
		}
	};

	// Spawn the monitoring thread
	m_monitoring_thread = std::thread(monitoring_loop);
}

void dcm::DCMDirectoryWatcher::RegisterFileCreateNotification(const OnDCMFileSystemChange& functor)
{
	if (functor)
	{
		m_create_callbacks.push_back(functor);
	}
}

void dcm::DCMDirectoryWatcher::RegisterFileChangeNotification(const OnDCMFileSystemChange& functor)
{
	if (functor)
	{
		m_change_callbacks.push_back(functor);
	}
}

void dcm::DCMDirectoryWatcher::RegisterFileDeleteNotification(const OnDCMFileSystemChange& functor)
{
	if (functor)
	{
		m_delete_callbacks.push_back(functor);
	}
}

void dcm::DCMDirectoryWatcher::RefreshDirectoryTree(const std::filesystem::path& path)
{
	if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (std::filesystem::is_directory(entry.status()))
			{
				// Recurse into subdirectory
				RefreshDirectoryTree(entry);
			}
			else if (std::filesystem::is_regular_file(entry.status()))
			{
				// This section will update the file list from a different thread, could be dangerous
				std::lock_guard<std::mutex> lock(m_file_list_mutex);

				// Redundant but helps to show the critical section
				{
					auto absolute_path = std::filesystem::absolute(entry.path());
					auto last_modified = std::filesystem::last_write_time(entry.path());
					auto absolute_path_name = absolute_path.generic_string();

					auto result = m_file_list.find(absolute_path_name);

					if (result == m_file_list.end())
					{
						// File did not exist, add it to the map
						m_file_list.insert({ absolute_path_name, last_modified });

						for (const auto& functor : m_create_callbacks)
						{
							functor(absolute_path_name);
						}
					}
					else
					{
						// File still exists, update the timestamp if the current timestamp is newer
						if (result->second != last_modified)
						{
							m_file_list[absolute_path_name] = last_modified;

							for (const auto& functor : m_change_callbacks)
							{
								functor(absolute_path_name);
							}
						}
					}

					// Mark files for delete (cannot remove items from the directory that is being iterated over)
					std::vector<std::string> keys_to_remove;
					for (const auto& file_ref : m_file_list)
					{
						if (!std::filesystem::exists(file_ref.first))
						{
							keys_to_remove.push_back(file_ref.first);
						}
					}

					// Remove old references
					for (const auto& outdated : keys_to_remove)
					{
						m_file_list.erase(outdated);

						for (const auto& functor : m_delete_callbacks)
						{
							functor(outdated);
						}
					}
				}
			}
		}
	}
}

void dcm::DCMDirectoryWatcher::StopWatching()
{
	if (m_is_listening)
	{
		m_is_listening = false;

		// Spin on the atomic until the thread shuts down
		while (m_is_listening) {}

		m_monitoring_thread.join();
	}
}
