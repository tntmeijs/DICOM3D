#ifndef DICOM3D_GL_SHADER_HPP
#define DICOM3D_GL_SHADER_HPP

#include "core/directory_watcher.hpp"

// OpenGL
#include <GL/gl3w.h>

// C++ standard
#include <array>
#include <atomic>
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
		/**
		 * Register the specified files as shader sources for this shader program
		 */
		DCMGLShader(const std::vector<std::string_view>& shader_sources);
		~DCMGLShader();

		/**
		 * Create new OpenGL shaders using the specified shader sources
		 */
		void Create();

		/**
		 * Start watching the source files for changes to hit reload shaders
		 * 
		 * Reloading is done synchronously, so it may cause a slight freeze while compiling shaders
		 */
#ifdef _DEBUG
		void StartHotReload(DCMDirectoryWatcher& directory_watcher);
#endif

		/**
		 * Activate the shader program
		 * 
		 * In debug mode this function will compile the shader again if changes are detected
		 * Unfortunately this cannot be done on a separate thread because OpenGL does not like more than one thread
		 */
		void Use();

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

#ifdef _DEBUG
		// Should this shader be reloaded?
		std::atomic_bool m_is_outdated;
#endif

		// Shader source file paths
		std::vector<std::string_view> m_shader_sources;
	};
}

#endif //! DICOM3D_GL_SHADER_HPP
