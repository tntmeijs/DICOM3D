#include <iostream>
#include <gl/gl3w.h>
#include <glfw/glfw3.h>

// Core
#include "core/window.hpp"

// Renderer
#include "renderer/renderer.hpp"

constexpr int VERSION_MAJOR = 4;
constexpr int VERSION_MINOR = 6;
constexpr int DEFAULT_WINDOW_WIDTH = 1280;
constexpr int DEFAULT_WINDOW_HEIGHT = 720;

int main()
{
	dcm::DCMRenderer renderer;
	dcm::DCMWindow window;

	glfwInit();

	window.Create(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "DICOM3D");
	renderer.Initialize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, VERSION_MAJOR, VERSION_MINOR);

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
		renderer.DrawFrame();
		window.NextFrame();
	}

	renderer.CleanUp();

	glfwTerminate();

    return 0;
}