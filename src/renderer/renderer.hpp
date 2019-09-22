#ifndef DICOM3D_RENDERER_HPP
#define DICOM3D_RENDERER_HPP

#include "gl_shader.hpp"

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
		 * Render the scene
		 */
		void DrawFrame() const;

		/**
		 * Clean-up resources
		 */
		void CleanUp();

	private:
		int m_gl_major;
		int m_gl_minor;
		int m_width;
		int m_height;

		// Temporary: handle to the compute shader output texture
		GLuint m_volumetric_output_texture;
		GLuint m_dummy_vao;

		DCMGLShader m_volumetric_shader;
		DCMGLShader m_fullscreen_triangle_shader;
	};
}

#endif //! DICOM3D_RENDERER_HPP
