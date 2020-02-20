#ifndef DICOM3D_DIRECTORY_WATCHER_WINDOWS_HPP
#define DICOM3D_DIRECTORY_WATCHER_WINDOWS_HPP

// Windows APIs
#include <Windows.h>

// C++ standard
#include <atomic>
#include <string_view>
#include <string>
#include <thread>

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
		 * Also monitors all subdirectories
		 * 
		 * Does not spawn a watch thread until "StartWatching()" is called explicitly
		 * 
		 * @param directory		The directory to watch
		 * @sa					StartWatching
		 */
		DCMDirectoryWatcherWindows(std::string_view directory);
		
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
		enum class DCMDirectoryUpdateType
		{
			FileFolderUpdated,	// When a new folder is updated (e.g. the files in the folder change)
			FileRenamed,		// When a file is renamed, created, removed
			FolderRenamed		// When a folder is renamed, created, removed
		};

		/**
		 * Function called from within the monitoring thread to update the directory listing
		 * Handle will be reset to its starting value, this allows it to listen for a new change
		 * 
		 * update_type	Indicates the type of update that was detected in the file system
		 * handle		Handle to the change notification handle
		 */
		void UpdateDirectoryListing(DCMDirectoryUpdateType update_type, HANDLE& handle);

	private:
		HANDLE m_file_folder_updated_handle;
		HANDLE m_file_renamed_handle;
		HANDLE m_folder_renamed_handle;

		/** Keeps the monitoring loop alive while true */
		std::atomic_bool m_is_listening;

		/** Thread that will constantly listen for file system changes */
		std::thread m_monitoring_thread;

		/** Directory to monitor */
		std::string m_directory_to_monitor;
	};
}

#endif //! DICOM3D_DIRECTORY_WATCHER_HPP
