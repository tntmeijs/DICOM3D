#include "gl_texture_2d.hpp"

dcm::DCMGLTexture2D::DCMGLTexture2D() :
	m_texture_handle(0),
	m_destroyed(false)
{
}

dcm::DCMGLTexture2D::~DCMGLTexture2D()
{
	Destroy();
}

void dcm::DCMGLTexture2D::CreateTexture(const DCMGLTexture2DInfo& create_info)
{
	// Get the OpenGL base format to store the texture in (used for pixel data)
	DCMGLFormat base_format = SizedFormatToBaseFormat(create_info.format);

	glGenTextures(1, &m_texture_handle);
	glBindTexture(GL_TEXTURE_2D, m_texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(create_info.wrap_s));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(create_info.wrap_t));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(create_info.min_filter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(create_info.mag_filter));
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		static_cast<GLint>(create_info.format),
		create_info.width,
		create_info.height,
		0,
		static_cast<GLenum>(base_format),
		static_cast<GLenum>(create_info.data_format),
		create_info.data);

	// Unbind to ensure the OpenGL state machine has been reset to a "default" value
	glBindTexture(GL_TEXTURE_2D, 0);
}

void dcm::DCMGLTexture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_texture_handle);
}

void dcm::DCMGLTexture2D::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint dcm::DCMGLTexture2D::Handle() const
{
	return m_texture_handle;
}

void dcm::DCMGLTexture2D::Destroy()
{
	if (!m_destroyed)
	{
		glDeleteTextures(1, &m_texture_handle);
		m_destroyed = true;
	}
}