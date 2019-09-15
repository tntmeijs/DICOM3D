#include "window.hpp"

dcm::DCMWindow::DCMWindow() :
	m_width(0),
	m_height(0),
	m_window(nullptr)
{
}

dcm::DCMWindow::~DCMWindow()
{
}

void dcm::DCMWindow::Create(int initial_width, int initial_height, std::string_view name)
{
	m_window = glfwCreateWindow(initial_width, initial_height, name.data(), nullptr, nullptr);

	if (!m_window)
	{
		spdlog::critical("Failed to create a GLFW window.");
		glfwTerminate();
		return;
	}

	spdlog::info("Successfully created a GLFW window.");

	// Save a pointer to this class to allow for access in the static functions
	glfwSetWindowUserPointer(m_window, this);

	// Register callbacks
	glfwSetKeyCallback(m_window, &GLFWKeyCallback);

	glfwMakeContextCurrent(m_window);
}

bool dcm::DCMWindow::IsRunning() const
{
	return !glfwWindowShouldClose(m_window);
}

void dcm::DCMWindow::StopRunning() const
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

void dcm::DCMWindow::NextFrame() const
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void dcm::DCMWindow::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	scancode, mods;
	const DCMWindow* const this_window = static_cast<DCMWindow*>(glfwGetWindowUserPointer(window));

	// Pass the key input to the public key callback function
	this_window->on_keyboard_input(key, action);
}
