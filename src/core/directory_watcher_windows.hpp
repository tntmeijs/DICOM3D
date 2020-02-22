#ifndef DICOM3D_DIRECTORY_WATCHER_WINDOWS_HPP
#define DICOM3D_DIRECTORY_WATCHER_WINDOWS_HPP

// C++ standard
#include <atomic>
#include <chrono>
#include <filesystem>
#include <mutex>
#include <string_view>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace dcm
{
	/**
	 * Windows-only class used to monitor file and directory changes
	 */
	class DCMDirectoryWatcherWindows
	{
	public:
		/**
		 * Create a new directory watcher and start monitoring the specified directory for changes
		 * 
		 * Does not spawn a watch thread until "StartWatching()" is called explicitly
		 * 
		 * @param directory			The directory to watch
		 * @param scan_interval		The interval in milliseconds between directory scans
		 * @sa						StartWatching
		 */
		DCMDirectoryWatcherWindows(std::string_view directory, std::uint32_t scan_interval);
		
		DCMDirectoryWatcherWindows(const DCMDirectoryWatcherWindows& other) = delete;

		/**
		 * Closes all open handles and stops the directory watching logic
		 */
		~DCMDirectoryWatcherWindows();

		/**
		 * Start monitoring all subdirectories for folder and file changes
		 * 
		 * @returns True on success, false on failure
		 */
		bool StartWatching();

		/**
		 * Stop monitoring the directory and close the thread
		 */
		void StopWatching();

	public:
		//#TODO: Add callbacks here whenever the file system updates!

	private:
		/**
		 * Scan the directory recursively for changes
		 * 
		 * @param path	Directory to scan for files and subdirectories
		 */
		void RefreshDirectoryTree(const std::filesystem::path& path);

	private:
		/** Keeps the monitoring loop alive while true */
		std::atomic_bool m_is_listening;

		/** Thread that will constantly listen for file system changes */
		std::thread m_monitoring_thread;

		/** Directory to monitor */
		std::string m_directory_to_monitor;

		/** Delay between directory scans */
		std::uint32_t m_scan_interval;

		/** List of all files and their last modified timestamp */
		std::unordered_map<std::string, std::filesystem::file_time_type> m_file_list;
		std::mutex m_file_list_mutex;
	};
}

#endif //! DICOM3D_DIRECTORY_WATCHER_HPP
