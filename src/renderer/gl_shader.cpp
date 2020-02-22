#include "gl_shader.hpp"

// Spdlog
#include <spdlog/spdlog.h>

// C++ standard
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>

dcm::DCMGLShader::DCMGLShader(const std::vector<std::string_view>& shader_sources) :
	m_program_handle(0),
	m_is_outdated(true),
	m_destroyed(false),
	m_shader_sources(shader_sources)
{
}

dcm::DCMGLShader::~DCMGLShader()
{
	Destroy();
}

void dcm::DCMGLShader::Create()
{
	// Handles to the shaders that will be created in the for-loop down below
	std::vector<GLuint> shader_handles;

	for (const auto& source : m_shader_sources)
	{
		// Extract information about this shader
		std::string extension = source.substr(source.find_last_of('.') + 1).data();
		DCMGLShaderType type = GetShaderTypeFromExtension(extension);
		
		// Shader source code
		std::string source_code = ReadShaderSourceCode(source);

		// Create the shader and save the result in the shader container
		shader_handles.push_back(CreateShaderFromSource(source_code, type));
	}

	// Create the shader program
	CreateShaderProgram(shader_handles);

	// Shaders are no longer necessary, whether or not the shaders compiled or program linked
	for (GLuint handle : shader_handles)
	{
		glDeleteShader(handle);
	}
}

#ifdef _DEBUG
void dcm::DCMGLShader::StartHotReload(DCMDirectoryWatcher& directory_watcher)
{
	directory_watcher.RegisterFileChangeNotification([&](std::string_view file)
		{
			// Did one of the shader source change?
			for (const auto source : m_shader_sources)
			{
				auto shader_path = std::filesystem::path(source);

				if (std::filesystem::equivalent(file, shader_path))
				{
					m_is_outdated = true;
				}
			}
		});
}
#endif

void dcm::DCMGLShader::Use()
{
#ifdef _DEBUG
	// Reload shader
	if (m_is_outdated)
	{
		Create();
		m_is_outdated = false;
	}
#endif

	glUseProgram(m_program_handle);
}

GLuint dcm::DCMGLShader::Handle() const
{
	return m_program_handle;
}

void dcm::DCMGLShader::Destroy()
{
	if (!m_destroyed)
	{
		glDeleteProgram(m_program_handle);
		m_destroyed = true;
	}
}

dcm::DCMGLShaderType dcm::DCMGLShader::GetShaderTypeFromExtension(std::string_view extension) const
{
	if		("vs" == extension) { return DCMGLShaderType::Vertex; }
	else if ("cs" == extension) { return DCMGLShaderType::Compute; }
	else if ("fs" == extension) { return DCMGLShaderType::Fragment; }
	else						{ return DCMGLShaderType::Invalid; }
}

std::string dcm::DCMGLShader::ReadShaderSourceCode(std::string_view path) const
{
	std::string source_code = "";
	std::ifstream file(path.data());

	if (!file)
	{
		spdlog::error("Cannot open shader: {}.", path);
		file.close();
		return source_code;
	}

	std::string buffer = "";
	while (std::getline(file, buffer))
	{
		source_code += (buffer + '\n');
	}

	source_code += '\0';

	file.close();
	return source_code;
}

GLuint dcm::DCMGLShader::CreateShaderFromSource(std::string_view source_code, DCMGLShaderType type) const
{
	GLuint shader = glCreateShader(static_cast<GLenum>(type));

	// Convert from std::string_view to const GLchar* const to make the function accept the source code
	const GLchar* const char_source = source_code.data();
	glShaderSource(shader, 1, &char_source, nullptr);

	// Compile the shader
	glCompileShader(shader);

	// Check for any issues
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (GL_FALSE == success)
	{
		GLint max_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> error_log(max_length);
		glGetShaderInfoLog(shader, max_length, nullptr, error_log.data());

		spdlog::error("Shader compile error: {}", error_log.data());
	}
	else
	{
		spdlog::info("Shader compiled successfully.");
	}

	return shader;
}

void dcm::DCMGLShader::CreateShaderProgram(const std::vector<GLuint>& handles)
{
	GLuint program_handle = glCreateProgram();

	// Attach the specified shaders to the program
	for (GLuint handle : handles)
	{
		glAttachShader(program_handle, handle);
	}

	// Link the program
	glLinkProgram(program_handle);

	// Check for any issues
	GLint success = 0;
	glGetProgramiv(program_handle, GL_LINK_STATUS, &success);

	if (GL_FALSE == success)
	{
		GLint max_length = 0;
		glGetProgramiv(program_handle, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> error_log(max_length);
		glGetProgramInfoLog(program_handle, max_length, nullptr, error_log.data());

		spdlog::error("Program link error: {}", error_log.data());

		// Do not leak the program
		glDeleteProgram(program_handle);
	}
	else
	{
		spdlog::info("Program linked successfully.");

		// Only replace the program handle when linking was successful
		m_program_handle = program_handle;
	}
}
