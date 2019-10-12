#include "renderer.hpp"

#include "gl_shader.hpp"

dcm::DCMRenderer::DCMRenderer() :
	m_gl_major(0),
	m_gl_minor(0),
	m_width(0),
	m_height(0),
	m_dummy_vao(0),
	m_volumetric_output_texture()
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

	// Create the volumetric compute shader output texture
	{
		DCMGLTexture2DInfo create_info = {};
		create_info.internal_format = GL_RGBA32F;
		create_info.format = GL_RGBA;
		create_info.type = GL_FLOAT;
		create_info.width = initial_width;
		create_info.height = initial_height;

		m_volumetric_output_texture.CreateTexture(create_info);
	}

	// DEBUG: test a volumetric texture
	{
		DCMGLTexture3DInfo create_info = {};
		create_info.internal_format = GL_R16I;
		create_info.format = GL_RED_INTEGER;
		create_info.type = GL_UNSIGNED_SHORT;
		create_info.slice_width = 256;
		create_info.slice_height = 256;
		create_info.paths =
		{
			"../MRbrain/MRbrain.1",
			"../MRbrain/MRbrain.2",
			"../MRbrain/MRbrain.3",
			"../MRbrain/MRbrain.4",
			"../MRbrain/MRbrain.5",
			"../MRbrain/MRbrain.6",
			"../MRbrain/MRbrain.7",
			"../MRbrain/MRbrain.8",
			"../MRbrain/MRbrain.9",
			"../MRbrain/MRbrain.10",
			"../MRbrain/MRbrain.11",
			"../MRbrain/MRbrain.12",
			"../MRbrain/MRbrain.13",
			"../MRbrain/MRbrain.14",
			"../MRbrain/MRbrain.15",
			"../MRbrain/MRbrain.16",
			"../MRbrain/MRbrain.17",
			"../MRbrain/MRbrain.18",
			"../MRbrain/MRbrain.19",
			"../MRbrain/MRbrain.20",
			"../MRbrain/MRbrain.21",
			"../MRbrain/MRbrain.22",
			"../MRbrain/MRbrain.23",
			"../MRbrain/MRbrain.24",
			"../MRbrain/MRbrain.25",
			"../MRbrain/MRbrain.26",
			"../MRbrain/MRbrain.27",
			"../MRbrain/MRbrain.28",
			"../MRbrain/MRbrain.29",
			"../MRbrain/MRbrain.30",
			"../MRbrain/MRbrain.31",
			"../MRbrain/MRbrain.32",
			"../MRbrain/MRbrain.33",
			"../MRbrain/MRbrain.34",
			"../MRbrain/MRbrain.35",
			"../MRbrain/MRbrain.36",
			"../MRbrain/MRbrain.37",
			"../MRbrain/MRbrain.38",
			"../MRbrain/MRbrain.39",
			"../MRbrain/MRbrain.40",
			"../MRbrain/MRbrain.41",
			"../MRbrain/MRbrain.42",
			"../MRbrain/MRbrain.43",
			"../MRbrain/MRbrain.44",
			"../MRbrain/MRbrain.45",
			"../MRbrain/MRbrain.46",
			"../MRbrain/MRbrain.47",
			"../MRbrain/MRbrain.48",
			"../MRbrain/MRbrain.49",
			"../MRbrain/MRbrain.50",
			"../MRbrain/MRbrain.51",
			"../MRbrain/MRbrain.52",
			"../MRbrain/MRbrain.53",
			"../MRbrain/MRbrain.54",
			"../MRbrain/MRbrain.55",
			"../MRbrain/MRbrain.56",
			"../MRbrain/MRbrain.57",
			"../MRbrain/MRbrain.58",
			"../MRbrain/MRbrain.59",
			"../MRbrain/MRbrain.60",
			"../MRbrain/MRbrain.61",
			"../MRbrain/MRbrain.62",
			"../MRbrain/MRbrain.63",
			"../MRbrain/MRbrain.64",
			"../MRbrain/MRbrain.65",
			"../MRbrain/MRbrain.66",
			"../MRbrain/MRbrain.67",
			"../MRbrain/MRbrain.68",
			"../MRbrain/MRbrain.69",
			"../MRbrain/MRbrain.70",
			"../MRbrain/MRbrain.71",
			"../MRbrain/MRbrain.72",
			"../MRbrain/MRbrain.73",
			"../MRbrain/MRbrain.74",
			"../MRbrain/MRbrain.75",
			"../MRbrain/MRbrain.76",
			"../MRbrain/MRbrain.77",
			"../MRbrain/MRbrain.78",
			"../MRbrain/MRbrain.79",
			"../MRbrain/MRbrain.80",
			"../MRbrain/MRbrain.81",
			"../MRbrain/MRbrain.82",
			"../MRbrain/MRbrain.83",
			"../MRbrain/MRbrain.84",
			"../MRbrain/MRbrain.85",
			"../MRbrain/MRbrain.86",
			"../MRbrain/MRbrain.87",
			"../MRbrain/MRbrain.88",
			"../MRbrain/MRbrain.89",
			"../MRbrain/MRbrain.90",
			"../MRbrain/MRbrain.91",
			"../MRbrain/MRbrain.92",
			"../MRbrain/MRbrain.93",
			"../MRbrain/MRbrain.94",
			"../MRbrain/MRbrain.95",
			"../MRbrain/MRbrain.96",
			"../MRbrain/MRbrain.97",
			"../MRbrain/MRbrain.98",
			"../MRbrain/MRbrain.99",
			"../MRbrain/MRbrain.100",
			"../MRbrain/MRbrain.101",
			"../MRbrain/MRbrain.102",
			"../MRbrain/MRbrain.103",
			"../MRbrain/MRbrain.104",
			"../MRbrain/MRbrain.105",
			"../MRbrain/MRbrain.106",
			"../MRbrain/MRbrain.107",
			"../MRbrain/MRbrain.108",
			"../MRbrain/MRbrain.109"
		};

		m_volumetric_texture_test.CreateTextureFromDataSlices(create_info);
	}

	// Dummy VAO
	glGenVertexArrays(1, &m_dummy_vao);
}

void dcm::DCMRenderer::DrawFrame() const
{
	constexpr int NUM_THREADS_X = 16;
	constexpr int NUM_THREADS_Y = 16;

	// === UPDATES ===
	m_volumetric_shader.Use();
	glBindImageTexture(0, m_volumetric_output_texture.Handle(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	glActiveTexture(GL_TEXTURE0);
	m_volumetric_texture_test.Bind();

	glDispatchCompute(static_cast<GLuint>(ceil(m_width / NUM_THREADS_X)), static_cast<GLuint>(ceil(m_height / NUM_THREADS_Y)), 1);

	// Wait for the compute shader to finish writing to the texture
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	// === DRAWING ===
	glClear(GL_COLOR_BUFFER_BIT);

	// Render a fullscreen triangle
	m_fullscreen_triangle_shader.Use();
	glActiveTexture(GL_TEXTURE0);
	m_volumetric_output_texture.Bind();
	glBindVertexArray(m_dummy_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void dcm::DCMRenderer::CleanUp()
{
	m_volumetric_shader.Destroy();
	m_fullscreen_triangle_shader.Destroy();
}
