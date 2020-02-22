#ifndef DICOM3D_RENDERER_HPP
#define DICOM3D_RENDERER_HPP

#include "gl_shader.hpp"
#include "gl_texture_2d.hpp"
#include "gl_texture_3d.hpp"
#include "core/transform.hpp"
#include "core/directory_watcher.hpp"

// Spdlog
#include "spdlog/spdlog.h"

// GL3W
#include <gl/gl3w.h>

namespace dcm
{
	/**
	 * OpenGL renderer
	 */
	class DCMRenderer
	{
	public:
		DCMRenderer();
		~DCMRenderer();

		/**
		 * Initialize OpenGL
		 */
		void Initialize(int initial_width, int initial_height, int gl_major, int gl_minor);

		/**
		 * Update the scene
		 */
		void Update(double delta_time);

		/**
		 * Render the scene
		 */
		void DrawFrame();

		/**
		 * Clean-up resources
		 */
		void CleanUp();

	private:
		int m_gl_major;
		int m_gl_minor;
		int m_width;
		int m_height;

		// Compute shader output texture
		DCMGLTexture2D m_volumetric_output_texture;

		// DEBUG: test a volumetric texture
		DCMGLTexture3D m_volumetric_texture_test;

		// Dummy VAO needed to make the "fullscreen triangle without buffers" trick work
		GLuint m_dummy_vao;

		// Camera transform
		DCMTransform m_camera_transform;

		DCMGLShader m_volumetric_shader;
		DCMGLShader m_fullscreen_triangle_shader;

		// Directory watcher is needed to monitor the shader files
		DCMDirectoryWatcher m_directory_watcher;
	};
}

#endif //! DICOM3D_RENDERER_HPP
