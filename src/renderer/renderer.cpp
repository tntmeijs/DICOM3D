#include "renderer.hpp"

#include "gl_shader.hpp"

dcm::DCMRenderer::DCMRenderer() :
	m_gl_major(0),
	m_gl_minor(0),
	m_width(0),
	m_height(0)
{
}

dcm::DCMRenderer::~DCMRenderer()
{
}

void dcm::DCMRenderer::Initialize(int initial_width, int initial_height, int gl_major, int gl_minor)
{
	m_gl_major = gl_major;
	m_gl_minor = gl_minor;
	m_width = initial_width;
	m_height = initial_height;

	// Configure GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_gl_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Initialize GL3W
	if (gl3wInit())
	{
		spdlog::critical("Failed to initialize GL3W.");
		glfwTerminate();
		return;
	}

	// OpenGL version not supported
	if (!gl3wIsSupported(4, 6))
	{
		spdlog::critical("OpenGL %i.%i not supported.", m_gl_major, m_gl_minor);
		glfwTerminate();
		return;
	}

	// Configure OpenGL
	glViewport(0, 0, m_width, m_height);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Create the full-screen output shader
	m_fullscreen_triangle_shader.Create({ "./resources/shaders/fullscreen_triangle.vs", "./resources/shaders/fullscreen_triangle.fs" });
}

void dcm::DCMRenderer::DrawFrame() const
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Render a fullscreen triangle
	m_fullscreen_triangle_shader.Use();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void dcm::DCMRenderer::CleanUp()
{
	m_fullscreen_triangle_shader.Destroy();
}
