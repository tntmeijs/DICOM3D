#ifndef DICOM3D_GL_FORMAT_HPP
#define DICOM3D_GL_FORMAT_HPP

// OpenGL
#include <GL/gl3w.h>

namespace dcm
{
	/**
	 * Texture wrap modes
	 */
	enum class DCMGLTextureWrapMode
	{
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER
	};

	/**
	 * Texture filtering modes
	 */
	enum class DCMGLTextureFilterMode
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR
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

	enum class DCMGLFormat
	{
		// Base OpenGL internal formats
		R = GL_RED,
		RG= GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,

		// Sized OpenGL formats
		R8 = GL_R8,
		R16 = GL_R16,
		R8_SNORM = GL_R8_SNORM,
		R16_SNORM = GL_R16_SNORM,
		
		RG8 = GL_RG8,
		RG16 = GL_RG16,
		RG8_SNORM = GL_RG8_SNORM,
		RG16_SNORM = GL_RG16_SNORM,
		
		RGB8 = GL_RGB8,
		RGB16 = GL_RGB16,
		RGB8_SNORM = GL_RGB8_SNORM,
		RGB16_SNORM = GL_RGB16_SNORM,

		R16F = GL_R16F,
		RG16F = GL_RG16F,
		RGB16F = GL_RGB16F,
		RGBA16F = GL_RGBA16F,

		R32F = GL_R32F,
		RG32F = GL_RG32F,
		RGB32F = GL_RGB32F,
		RGBA32F = GL_RGBA32F,
	};

	/**
	 * Sized format to base OpenGL format
	 */
	static constexpr DCMGLFormat SizedFormatToBaseFormat(DCMGLFormat format)
	{
		switch (format)
		{
		case dcm::DCMGLFormat::R8:
		case dcm::DCMGLFormat::R16:
		case dcm::DCMGLFormat::R8_SNORM:
		case dcm::DCMGLFormat::R16_SNORM:
		case dcm::DCMGLFormat::R16F:
		case dcm::DCMGLFormat::R32F:
		{
			return DCMGLFormat::R;
		} break;

		case dcm::DCMGLFormat::RG8:
		case dcm::DCMGLFormat::RG16:
		case dcm::DCMGLFormat::RG8_SNORM:
		case dcm::DCMGLFormat::RG16_SNORM:
		case dcm::DCMGLFormat::RG16F:
		case dcm::DCMGLFormat::RG32F:
		{
			return DCMGLFormat::RG;
		} break;
		
		case dcm::DCMGLFormat::RGB8:
		case dcm::DCMGLFormat::RGB16:
		case dcm::DCMGLFormat::RGB8_SNORM:
		case dcm::DCMGLFormat::RGB16_SNORM:
		case dcm::DCMGLFormat::RGB16F:
		case dcm::DCMGLFormat::RGB32F:
		{
			return DCMGLFormat::RGB;
		} break;

		case dcm::DCMGLFormat::RGBA16F:
		case dcm::DCMGLFormat::RGBA32F:
		{
			return DCMGLFormat::RGBA;
		} break;

		default:
			break;
		}

		// No base format found, just pass the input as the output
		return format;
	}
}

#endif //! DICOM3D_GL_FORMAT_HPP
