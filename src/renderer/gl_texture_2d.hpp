#ifndef DICOM3D_GL_TEXTURE_2D_HPP
#define DICOM3D_GL_TEXTURE_2D_HPP

#include "gl_format.hpp"

// OpenGL
#include <GL/gl3w.h>

// C++ standard
#include <string_view>

namespace dcm
{
	/**
	 * Texture wrap modes
	 */
	enum class DCMGLTextureWrapMode
	{
		Repeat			= GL_REPEAT,
		MirroredRepeat	= GL_MIRRORED_REPEAT,
		ClampToEdge		= GL_CLAMP_TO_EDGE,
		ClampToBorder	= GL_CLAMP_TO_BORDER
	};

	/**
	 * Texture filtering modes
	 */
	enum class DCMGLTextureFilterMode
	{
		Nearest	= GL_NEAREST,
		Linear	= GL_LINEAR
	};

	/**
	 * Texture data format
	 */
	enum class DCMGLTextureDataFormat
	{
		UByte = GL_UNSIGNED_BYTE,
		Byte = GL_BYTE,
		UShort = GL_UNSIGNED_SHORT,
		Short = GL_SHORT,
		UInt = GL_UNSIGNED_INT,
		Int = GL_INT,
		Float16 = GL_HALF_FLOAT,
		Float = GL_FLOAT,
	};

	/**
	 * Information needed to create a texture
	 */
	struct DCMGLTexture2DInfo
	{
		// Texture data
		const void* data = nullptr;
		
		// Texture internal format
		DCMGLFormat format = DCMGLFormat::RGBA;

		// Pixel data format
		DCMGLTextureDataFormat data_format = DCMGLTextureDataFormat::UByte;

		// Texture dimensions
		std::uint32_t width = 0;
		std::uint32_t height = 0;

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
		void CreateTexture(const DCMGLTexture2DInfo& create_info);

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
