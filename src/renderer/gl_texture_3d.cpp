#include "gl_texture_3d.hpp"

// STB image
#include <stb_image.h>

// Spdlog
#include <spdlog/spdlog.h>

// C++ standard
#include <fstream>

dcm::DCMGLTexture3D::DCMGLTexture3D() :
	m_texture_handle(0),
	m_destroyed(false)
{
}

dcm::DCMGLTexture3D::~DCMGLTexture3D()
{
	Destroy();
}

void dcm::DCMGLTexture3D::CreateTextureFromImageSlices(const DCMGLTexture3DInfo& create_info)
{
	if (0 == create_info.paths.size())
	{
		spdlog::error("Cannot create a 3D texture without any slices.");
		return;
	}

	// Number of bytes required to store a pixel
	std::uint32_t slice_bytes_per_pixel = FormatToBytesPerPixel(create_info.internal_format);

	// Size of a single 2D slice
	std::uint32_t slice_size_bytes = slice_bytes_per_pixel * create_info.slice_width * create_info.slice_height;

	// Needed to store N 2D slices in memory (size of the 3D texture)
	unsigned char* const volume_data = new unsigned char[slice_bytes_per_pixel * slice_size_bytes * create_info.paths.size()];

	// Load each 2D slice and copy it to the volume data blob
	for (std::uint32_t i = 0; i < create_info.paths.size(); ++i)
	{
		std::string_view path = create_info.paths[i];

		int width = 0;
		int height = 0;
		int channel_count = 0;
		unsigned char* const data = stbi_load(path.data(), &width, &height, &channel_count, 0);

		// Actual width and / or height of the slice does not match the specified width and / or height
		if (width != create_info.slice_width || height != create_info.slice_height)
		{
			spdlog::error("Actual 2D slice width and / or height does not match specified width and / or height.");
			
			// Unable to load any data
			if (!data)
			{
				spdlog::error("Unable to load {}", path.data());
			}

			delete[] volume_data;
			return;
		}

		// Copy slice data to the volume data blob
		// Each slice has an offset of the number of previous slices to avoid overwriting existing slices
		memcpy(&volume_data[i * slice_size_bytes], data, slice_size_bytes);

		// Clean-up the slice data
		stbi_image_free(data);
	}

	// Create the 3D texture
	glGenTextures(1, &m_texture_handle);
	glTexImage3D(
		GL_TEXTURE_3D,
		0,
		create_info.internal_format,
		create_info.slice_width,
		create_info.slice_height,
		static_cast<GLsizei>(create_info.paths.size()),
		0,
		create_info.format,
		create_info.type,
		volume_data);

	// Clean-up the pixel data
	delete[] volume_data;

	spdlog::info("Successfully loaded 3D texture from 2D image slices.");
}

void dcm::DCMGLTexture3D::CreateTextureFromDataSlices(const DCMGLTexture3DInfo& create_info)
{
	if (0 == create_info.paths.size())
	{
		spdlog::error("Cannot create a 3D texture without any slices.");
		return;
	}

	// Number of bytes required to store a pixel
	std::uint32_t slice_bytes_per_pixel = FormatToBytesPerPixel(create_info.internal_format);

	// Size of a single 2D slice
	std::uint32_t slice_size_bytes = slice_bytes_per_pixel * create_info.slice_width * create_info.slice_height;

	// Needed to store N 2D slices in memory (size of the 3D texture)
	unsigned char* const volume_data = new unsigned char[slice_bytes_per_pixel * slice_size_bytes * create_info.paths.size()];

	// Load each 2D slice and copy it to the volume data blob
	for (std::uint32_t i = 0; i < create_info.paths.size(); ++i)
	{
		std::string_view path = create_info.paths[i];

		std::ifstream file(path.data(), std::ios_base::binary);

		if (!file)
		{
			file.close();
			delete[] volume_data;
			spdlog::error("Unable to load binary slice data.");
			return;
		}

		char* data = new char[slice_size_bytes];

		// Read the entire slice
		file.seekg(0);
		file.read(&data[0], slice_size_bytes);

		// Copy the slice to the volumetric data blob
		memcpy(&volume_data[i * slice_size_bytes], data, slice_size_bytes);

		// Clean-up
		delete[] data;
		file.close();
	}

	// Create the 3D texture
	glGenTextures(1, &m_texture_handle);
	glBindTexture(GL_TEXTURE_3D, m_texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage3D(
		GL_TEXTURE_3D,
		0,
		create_info.internal_format,
		create_info.slice_width,
		create_info.slice_height,
		static_cast<GLsizei>(create_info.paths.size()),
		0,
		create_info.format,
		create_info.type,
		volume_data);

	// Unbind to avoid leaving the state machine in a state
	glBindTexture(GL_TEXTURE_3D, 0);

	// Clean-up the pixel data
	delete[] volume_data;

	spdlog::info("Successfully loaded 3D texture from 2D image slices.");
}

void dcm::DCMGLTexture3D::Bind() const
{
	glBindTexture(GL_TEXTURE_3D, m_texture_handle);
}

void dcm::DCMGLTexture3D::Unbind() const
{
	glBindTexture(GL_TEXTURE_3D, 0);
}

GLuint dcm::DCMGLTexture3D::Handle() const
{
	return m_texture_handle;
}

void dcm::DCMGLTexture3D::Destroy()
{
	if (!m_destroyed)
	{
		glDeleteTextures(1, &m_texture_handle);
		m_destroyed = true;
	}
}
