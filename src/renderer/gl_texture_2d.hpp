#ifndef DICOM3D_GL_TEXTURE_2D_HPP
#define DICOM3D_GL_TEXTURE_2D_HPP

#include "gl_format.hpp"

// OpenGL
#include <GL/gl3w.h>

// C++ standard
#include <optional>
#include <string_view>

namespace dcm
{
	/**
	 * Information needed to create a texture
	 */
	struct DCMGLTexture2DInfo
	{
		// Texture path (if nullopt, an empty texture will be allocated --> perfect for compute output)
		std::optional<std::string_view> path;
		
		// Texture internal format
		DCMGLFormat format = DCMGLFormat::RGBA;

		// Pixel data format
		DCMGLTextureDataFormat data_format = DCMGLTextureDataFormat::UByte;

		// Texture dimensions (leave at zero if a path is specified)
		int width = 0;
		int height = 0;

		// Wrap modes
		DCMGLTextureWrapMode wrap_s = DCMGLTextureWrapMode::ClampToBorder;
		DCMGLTextureWrapMode wrap_t = DCMGLTextureWrapMode::ClampToBorder;

		// Filter modes
		DCMGLTextureFilterMode min_filter = DCMGLTextureFilterMode::Linear;
		DCMGLTextureFilterMode mag_filter = DCMGLTextureFilterMode::Linear;
	};

	/**
	 * 2D OpenGl texture wrapper class
	 */
	class DCMGLTexture2D
	{
	public:
		DCMGLTexture2D();
		~DCMGLTexture2D();

		/**
		 * Create a new texture
		 */
		void CreateTexture(DCMGLTexture2DInfo& create_info);

		/**
		 * Bind the texture to GL_TEXTURE_2D (does not active a texture slot)
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

#endif //! DICOM3D_GL_TEXTURE_2D_HPP
