#include "directory_watcher_windows.hpp"

// Spdlog
#include "spdlog/spdlog.h"

// C++ standard
#include <cstdint>
#include <iostream>

dcm::DCMDirectoryWatcherWindows::DCMDirectoryWatcherWindows(std::string_view directory, std::uint32_t scan_interval) :
	m_is_listening(true),
	m_directory_to_monitor(directory),
	m_scan_interval(scan_interval)
{
}

dcm::DCMDirectoryWatcherWindows::~DCMDirectoryWatcherWindows()
{
	StopWatching();
}

bool dcm::DCMDirectoryWatcherWindows::StartWatching()
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

	return true;
}

void dcm::DCMDirectoryWatcherWindows::RefreshDirectoryTree(const std::filesystem::path& path)
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

					auto result = m_file_list.find(absolute_path.generic_string());

					if (result == m_file_list.end())
					{
						// File did not exist, add it to the map
						m_file_list.insert({ absolute_path.generic_string(), last_modified });

						std::cout << "Discovered file: " << absolute_path << std::endl;
					}
					else
					{
						// File still exists, update the timestamp if the current timestamp is newer
						if (result->second != last_modified)
						{
							m_file_list[absolute_path.generic_string()] = last_modified;

							std::cout << "Updated file: " << absolute_path << std::endl;
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
						std::cout << outdated << " has been removed from the filesystem" << std::endl;
					}
				}
			}
		}
	}
}

void dcm::DCMDirectoryWatcherWindows::StopWatching()
{
	if (m_is_listening)
	{
		m_is_listening = false;

		// Spin on the atomic until the thread shuts down
		while (m_is_listening) {}

		m_monitoring_thread.join();
	}
}
