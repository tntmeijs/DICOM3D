#include "directory_watcher_windows.hpp"

// Spdlog
#include "spdlog/spdlog.h"

// C++ standard
#include <cstdint>

dcm::DCMDirectoryWatcherWindows::DCMDirectoryWatcherWindows(std::string_view directory) :
	m_file_folder_updated_handle(INVALID_HANDLE_VALUE),
	m_file_renamed_handle(INVALID_HANDLE_VALUE),
	m_folder_renamed_handle(INVALID_HANDLE_VALUE),
	m_is_listening(true),
	m_directory_to_monitor(directory)
{
}

dcm::DCMDirectoryWatcherWindows::~DCMDirectoryWatcherWindows()
{
	StopWatching();
}

bool dcm::DCMDirectoryWatcherWindows::StartWatching()
{
	m_file_folder_updated_handle	= FindFirstChangeNotification(m_directory_to_monitor.c_str(), true, FILE_NOTIFY_CHANGE_LAST_WRITE);
	m_file_renamed_handle			= FindFirstChangeNotification(m_directory_to_monitor.c_str(), true, FILE_NOTIFY_CHANGE_FILE_NAME);
	m_folder_renamed_handle			= FindFirstChangeNotification(m_directory_to_monitor.c_str(), true, FILE_NOTIFY_CHANGE_DIR_NAME);

	if (m_file_folder_updated_handle	== INVALID_HANDLE_VALUE ||
		m_file_renamed_handle			== INVALID_HANDLE_VALUE ||
		m_folder_renamed_handle			== INVALID_HANDLE_VALUE)
	{
		spdlog::critical("Unable to create monitoring handles for the directory.");
		return false;
	}
	else
	{
		spdlog::info("Directory monitoring handles set.");
	}

	auto monitoring_loop = [&]()
	{
		HANDLE handles[] =
		{
			m_file_folder_updated_handle,	// WAIT_OBJECT_0
			m_file_renamed_handle,			// WAIT_OBJECT_0 + 1
			m_folder_renamed_handle			// WAIT_OBJECT_0 + 2
		};

		std::uint32_t handle_count = sizeof(handles) / sizeof(HANDLE);

		while (m_is_listening)
		{
			// Wait until a signal comes through
			DWORD wait_status = WaitForMultipleObjects(4, handles, false, INFINITE);

			switch (wait_status)
			{
			case WAIT_OBJECT_0:
				UpdateDirectoryListing(DCMDirectoryUpdateType::FileFolderUpdated, m_file_folder_updated_handle);
				break;

			case WAIT_OBJECT_0 + 1:
				UpdateDirectoryListing(DCMDirectoryUpdateType::FileRenamed, m_file_renamed_handle);
				break;

			case WAIT_OBJECT_0 + 2:
				UpdateDirectoryListing(DCMDirectoryUpdateType::FolderRenamed, m_folder_renamed_handle);
				break;

			default:
				break;
			}
		}
	};

	// Spawn the monitoring thread
	m_monitoring_thread = std::thread(monitoring_loop);

	return true;
}

void dcm::DCMDirectoryWatcherWindows::UpdateDirectoryListing(DCMDirectoryUpdateType update_type, HANDLE& handle)
{
	// Restart the notification
	if (!FindNextChangeNotification(handle))
	{
		spdlog::critical("\"FindNextChangeNotification\" function failed, monitoring thread will now shut-down");
		StopWatching();
	}

	// Handle the change
	switch (update_type)
	{
	case dcm::DCMDirectoryWatcherWindows::DCMDirectoryUpdateType::FileFolderUpdated:
		spdlog::info("file / folder updated");
		break;
	case dcm::DCMDirectoryWatcherWindows::DCMDirectoryUpdateType::FileRenamed:
		spdlog::info("file renamed");
		break;
	case dcm::DCMDirectoryWatcherWindows::DCMDirectoryUpdateType::FolderRenamed:
		spdlog::info("folder renamed");
		break;
	default:
		spdlog::info("unknown operation");
		break;
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
