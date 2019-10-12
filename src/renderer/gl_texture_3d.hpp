#ifndef DICOM3D_GL_TEXTURE_3D_HPP
#define DICOM3D_GL_TEXTURE_3D_HPP

#include "gl_format.hpp"

// OpenGL
#include <GL/gl3w.h>

// C++ standard
#include <string_view>
#include <vector>

namespace dcm
{
	/**
	 * Data needed to create a 3D texture
	 */
	struct DCMGLTexture3DInfo
	{
		// Width of a single slice in pixels
		int slice_width = 0;

		// Height of a single slice in pixels
		int slice_height = 0;

		// Internal texture GL format
		GLint internal_format;

		// Texture GL format
		GLenum format;

		// Pixel data type
		GLenum type;

		// Paths to each slice
		std::vector<std::string_view> paths;
	};

	/**
	 * Class that wraps 3D texture creation
	 */
	class DCMGLTexture3D
	{
	public:
		DCMGLTexture3D();
		~DCMGLTexture3D();

		/**
		 * Create a new 3D texture from 2D images
		 */
		void CreateTextureFromImageSlices(const DCMGLTexture3DInfo& create_info);

		/**
		 * Create a new 3D texture from 2D slices (binary data files, no headers, just data)
		 */
		void CreateTextureFromDataSlices(const DCMGLTexture3DInfo& create_info);

		/**
		 * Bind the texture to GL_TEXTURE_3D (does not active a texture slot)
		 */
		void Bind() const;

		/**
		 * Unbind the texture
		 */
		void Unbind() const;

		/**
		 * Get the texture handle
		 */
		GLuint Handle() const;

		/**
		 * Clean-up allocated resources
		 */
		void Destroy();

	private:
		GLuint m_texture_handle;

		bool m_destroyed;
	};
}

#endif //! DICOM3D_GL_TEXTURE_3D_HPP
