#include <iostream>
#include <gl/gl3w.h>
#include <glfw/glfw3.h>

// Make STB files work
#include "stb_defines.hpp"

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
		renderer.Update(1.0);
		renderer.DrawFrame();
		window.NextFrame();
	}

	renderer.CleanUp();

	glfwTerminate();

    return 0;
}