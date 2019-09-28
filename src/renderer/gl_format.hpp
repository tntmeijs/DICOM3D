#ifndef DICOM3D_GL_FORMAT_HPP
#define DICOM3D_GL_FORMAT_HPP

// OpenGL
#include <GL/gl3w.h>

// C++ standard
#include <cstdint>

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
	 * Texture pixel data format
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

	enum class DCMGLTextureChannelFormat
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
	static constexpr DCMGLTextureChannelFormat SizedFormatToBaseFormat(DCMGLTextureChannelFormat channel_format)
	{
		switch (channel_format)
		{
		case dcm::DCMGLTextureChannelFormat::R8:
		case dcm::DCMGLTextureChannelFormat::R16:
		case dcm::DCMGLTextureChannelFormat::R8_SNORM:
		case dcm::DCMGLTextureChannelFormat::R16_SNORM:
		case dcm::DCMGLTextureChannelFormat::R16F:
		case dcm::DCMGLTextureChannelFormat::R32F:
		{
			return DCMGLTextureChannelFormat::R;
		} break;

		case dcm::DCMGLTextureChannelFormat::RG8:
		case dcm::DCMGLTextureChannelFormat::RG16:
		case dcm::DCMGLTextureChannelFormat::RG8_SNORM:
		case dcm::DCMGLTextureChannelFormat::RG16_SNORM:
		case dcm::DCMGLTextureChannelFormat::RG16F:
		case dcm::DCMGLTextureChannelFormat::RG32F:
		{
			return DCMGLTextureChannelFormat::RG;
		} break;
		
		case dcm::DCMGLTextureChannelFormat::RGB8:
		case dcm::DCMGLTextureChannelFormat::RGB16:
		case dcm::DCMGLTextureChannelFormat::RGB8_SNORM:
		case dcm::DCMGLTextureChannelFormat::RGB16_SNORM:
		case dcm::DCMGLTextureChannelFormat::RGB16F:
		case dcm::DCMGLTextureChannelFormat::RGB32F:
		{
			return DCMGLTextureChannelFormat::RGB;
		} break;

		case dcm::DCMGLTextureChannelFormat::RGBA16F:
		case dcm::DCMGLTextureChannelFormat::RGBA32F:
		{
			return DCMGLTextureChannelFormat::RGBA;
		} break;

		default:
			break;
		}

		// No base format found, just pass the input as the output
		return channel_format;
	}

	/**
	 * Number of bytes per pixel
	 */
	static constexpr std::uint32_t DCMGLFormatToBytesPerPixel(DCMGLTextureChannelFormat channel_format, DCMGLTextureDataFormat data_format)
	{
		DCMGLTextureChannelFormat channel_type = SizedFormatToBaseFormat(channel_format);

		std::uint32_t channels_per_pixel = 1;
		std::uint32_t bytes_per_channel = 1;

		// Number of channels per pixel
		switch (channel_type)
		{
		
		case dcm::DCMGLTextureChannelFormat::R:
		{
			channels_per_pixel = 1;
		} break;
		
		case dcm::DCMGLTextureChannelFormat::RG:
		{
			channels_per_pixel = 2;
		} break;
		
		case dcm::DCMGLTextureChannelFormat::RGB:
		{
			channels_per_pixel = 3;
		} break;
		
		case dcm::DCMGLTextureChannelFormat::RGBA:
		{
			channels_per_pixel = 4;
		} break;
		
		default:
			break;
		}

		// Size of a data format in bytes
		switch (data_format)
		{
		case dcm::DCMGLTextureDataFormat::UByte:
		case dcm::DCMGLTextureDataFormat::Byte:
		{
			bytes_per_channel = 1;
		} break;

		case dcm::DCMGLTextureDataFormat::UShort:
		case dcm::DCMGLTextureDataFormat::Short:
		{
			bytes_per_channel = 2;
		} break;
		
		case dcm::DCMGLTextureDataFormat::UInt:
		case dcm::DCMGLTextureDataFormat::Int:
		{
			bytes_per_channel = 4;
		} break;
		
		case dcm::DCMGLTextureDataFormat::Float16:
		{
			bytes_per_channel = 2;
		} break;
		
		case dcm::DCMGLTextureDataFormat::Float:
		{
			bytes_per_channel = 4;
		} break;
		
		default:
			break;
		}

		// Total bytes per pixel = channel count * bytes per channel
		return channels_per_pixel * bytes_per_channel;
	}
}

#endif //! DICOM3D_GL_FORMAT_HPP
