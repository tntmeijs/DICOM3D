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

	inline std::uint32_t FormatToBytesPerPixel(GLint format)
	{
		switch (format)
		{
		case GL_R8:
		case GL_R8_SNORM:
		case GL_R8I:
		case GL_R8UI:
		{
			return 1;
		} break;

		case GL_R16:
		case GL_R16_SNORM:
		case GL_R16F:
		case GL_R16I:
		case GL_RG8:
		case GL_RG8_SNORM:
		case GL_RG8I:
		case GL_RG8UI:
		{
			return 2;
		} break;

		case GL_RGB8:
		case GL_RGB8_SNORM:
		case GL_RGB8I:
		case GL_RGB8UI:
		{
			return 3;
		} break;

		case GL_RG16:
		case GL_RG16_SNORM:
		case GL_RG16F:
		case GL_RG16I:
		case GL_RGBA8:
		case GL_RGBA8_SNORM:
		case GL_RGBA8I:
		case GL_RGBA8UI:
		{
			return 4;
		} break;

		case GL_RGB16:
		case GL_RGB16_SNORM:
		case GL_RGB16F:
		case GL_RGB16I:
		{
			return 6;
		} break;

		case GL_RGBA16:
		case GL_RGBA16_SNORM:
		case GL_RGBA16F:
		case GL_RGBA16I:
		{
			return 8;
		} break;

		default:
			break;
		}

		return 0;
	}
}

#endif //! DICOM3D_GL_FORMAT_HPP
