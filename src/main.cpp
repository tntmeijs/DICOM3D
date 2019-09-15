#include <iostream>
#include <gl/gl3w.h>
#include <glfw/glfw3.h>

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

	GLFWwindow* window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "DICOM3D", nullptr, nullptr);
	
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

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

	std::cout << "OpenGL " << glGetString(GL_VERSION) << ", GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	std::cin.get();

    return 0;
}