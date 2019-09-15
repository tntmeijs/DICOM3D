#ifndef DICOM3D_WINDOW_HPP
#define DICOM3D_WINDOW_HPP

// Spdlog
#include <spdlog/spdlog.h>

// GLFW
#include <glfw/glfw3.h>

// C++ standard
#include <functional>
#include <string_view>

namespace dcm
{
	/**
	 * Create and manage a window
	 */
	class DCMWindow
	{
	public:
		DCMWindow();
		~DCMWindow();

		/**
		 * Create a new window and make it the current context
		 */
		void Create(int initial_width, int initial_height, std::string_view name);

		/**
		 * Should the window be kept open?
		 */
		bool IsRunning() const;

		/**
		 * Request the window to be closed
		 */
		void StopRunning() const;

		/**
		 * Poll GLFW events and swap the back buffers
		 */
		void NextFrame() const;

	public:
		/**
		 * Key callback
		 */
		std::function<void(int key, int action)> on_keyboard_input;

	private:
		/**
		 * Key callback proxy to make the function more user-friendly for the end user
		 */
		static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	private:
		int m_width;
		int m_height;

		GLFWwindow* m_window;
	};
}

#endif //! DICOM3D_WINDOW_HPP
