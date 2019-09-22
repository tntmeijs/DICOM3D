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

	// Initialize GL3W
	if (gl3wInit())
	{
		spdlog::critical("Failed to initialize GL3W.");
		return;
	}

	// OpenGL version not supported
	if (!gl3wIsSupported(4, 6))
	{
		spdlog::critical("OpenGL %i.%i not supported.", m_gl_major, m_gl_minor);
		return;
	}

	// Configure OpenGL
	glViewport(0, 0, m_width, m_height);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Create the shader that will do the volumetric rendering
	m_volumetric_shader.Create({ "./resources/shaders/volumetric.cs" });

	// Create the full-screen output shader
	m_fullscreen_triangle_shader.Create({ "./resources/shaders/fullscreen_triangle.vs", "./resources/shaders/fullscreen_triangle.fs" });
	glUniform1i(glGetUniformLocation(m_fullscreen_triangle_shader.Handle(), "volumetricResult"), 0);	// Bind to GL_TEXTURE0

	// Temporary: create a texture to output the compute shader results to
	glGenTextures(1, &m_volumetric_output_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_volumetric_output_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, initial_width, initial_height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindImageTexture(0, m_volumetric_output_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	glGenVertexArrays(1, &m_dummy_vao);
}

void dcm::DCMRenderer::DrawFrame() const
{
	constexpr int NUM_THREADS_X = 16;
	constexpr int NUM_THREADS_Y = 16;

	// === UPDATES ===
	m_volumetric_shader.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_volumetric_output_texture);
	glDispatchCompute(static_cast<GLuint>(ceil(m_width / NUM_THREADS_X)), static_cast<GLuint>(ceil(m_height / NUM_THREADS_Y)), 1);

	// === DRAWING ===
	glClear(GL_COLOR_BUFFER_BIT);

	// Render a fullscreen triangle
	m_fullscreen_triangle_shader.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_volumetric_output_texture);
	glBindVertexArray(m_dummy_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void dcm::DCMRenderer::CleanUp()
{
	m_volumetric_shader.Destroy();
	m_fullscreen_triangle_shader.Destroy();
}
