#ifndef DICOM3D_GL_SHADER_HPP
#define DICOM3D_GL_SHADER_HPP

// OpenGL
#include <GL/gl3w.h>

// C++ standard
#include <array>
#include <string_view>
#include <string>
#include <vector>

namespace dcm
{
	/**
	 * Supported shader types
	 */
	enum class DCMGLShaderType
	{
		Invalid = -1,
		Vertex = GL_VERTEX_SHADER,
		Compute = GL_COMPUTE_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

	/**
	 * An OpenGL shader
	 */
	class DCMGLShader
	{
	public:
		DCMGLShader();
		~DCMGLShader();

		/**
		 * Create new OpenGL shaders using the specified shader sources
		 */
		void Create(const std::vector<std::string_view>& shader_sources);

		/**
		 * Activate the shader program
		 */
		void Use() const;

		/**
		 * Get the shader program handle
		 */
		GLuint Handle() const;

		/**
		 * Delete the shader program
		 */
		void Destroy();

	private:
		/**
		 * Get the shader type
		 */
		DCMGLShaderType GetShaderTypeFromExtension(std::string_view extension) const;

		/**
		 * Read the source code from a file
		 */
		std::string ReadShaderSourceCode(std::string_view path) const;

		/**
		 * Create a new shader out of the specified source code and type
		 */
		GLuint CreateShaderFromSource(std::string_view source_code, DCMGLShaderType type) const;

		/**
		 * Create a new shader program using the specified shader handles
		 */
		void CreateShaderProgram(const std::vector<GLuint>& handles);

	private:
		GLuint m_program_handle;

		// Flag that indicates whether this shader program has been destroyed already (cannot call "Destroy()" twice)
		bool m_destroyed;
	};
}

#endif //! DICOM3D_GL_SHADER_HPP
