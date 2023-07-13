#include "Texture2D.h"

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <iostream>
#include <core/Log.h>

Texture2D::Texture2D()
{
}

Texture2D::Texture2D(const std::string& path, bool pixelArtStyle)
{
	init(path, pixelArtStyle);
}

Texture2D::Texture2D(unsigned int id)
{
	mTexture = id;
}


Texture2D::~Texture2D()
{
	if (mTexture && mWidth != 0)
	{
		std::cout << "Deleted Texture " << mTexture << "!\n";
		glDeleteTextures(1, &mTexture);
	}
}
void Texture2D::init(const std::string& path, bool pixelArtStyle)
{
	if (!mTexture)
	{
		glGenTextures(1, &mTexture);
	}

	int channels;
	unsigned char* data;
	data = SOIL_load_image(path.c_str(), &mWidth, &mHeight, &channels, SOIL_LOAD_RGBA);
	if (!data)
	{
		LOG_ERROR("Failed to load texture " + path, false);
	}

	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)data);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(data);

	if (pixelArtStyle)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}
void Texture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, mTexture);
}
unsigned int Texture2D::id() const
{
	return mTexture;
}
glm::vec2 Texture2D::getSize() const
{
	return glm::vec2(mWidth, mHeight);
}