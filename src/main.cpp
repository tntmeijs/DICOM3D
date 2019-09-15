#include <iostream>
#include <gl/gl3w.h>
#include <glfw/glfw3.h>

#include "core/window.hpp"

constexpr int VERSION_MAJOR = 4;
constexpr int VERSION_MINOR = 6;
constexpr int DEFAULT_WINDOW_WIDTH = 1280;
constexpr int DEFAULT_WINDOW_HEIGHT = 720;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	dcm::DCMWindow window;
	window.Create(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "DICOM3D");

	if (gl3wInit())
	{
		std::cout << "Failed to initialize GL3W" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gl3wIsSupported(4, 6))
	{
		std::cout << "OpenGL " << VERSION_MAJOR << "." << VERSION_MINOR << " not supported." << std::endl;
		glfwTerminate();
		return -1;
	}

	window.on_keyboard_input = [&window](int key, int action)
	{
		// Exit the window when escape is pressed
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			window.StopRunning();
		}
	};

	// Main loop
	while (window.IsRunning())
	{
		window.NextFrame();
	}

	glfwTerminate();

    return 0;
}