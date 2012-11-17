// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com
//
// This class is based on Texture class from IrrlichtEngine (http://irrlicht.sourceforge.net/)


#include "Texture.h"
#include "ExtensionManager.h"

void Texture::disable()
{
	glDisable(GL_TEXTURE_2D);
}

Texture::Texture(void* data, unsigned short width, unsigned short height, unsigned short depth, bool mipMaps)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->mipMaps = mipMaps;

	textureFilter = BILINEAR_FILTER;

	id = 0;

	GLint internal_format;
	GLenum format;
	GLenum type;

	switch (depth)
	{
	case 8:
		internal_format = GL_RGBA;
		format = GL_LUMINANCE;
		type = GL_UNSIGNED_BYTE;
		break;

	case 16:
		internal_format = GL_RGB;
		format = GL_RGB;
		type = GL_UNSIGNED_SHORT_5_6_5;
		break;

	case 24:
		internal_format = GL_RGB8;
		format = GL_RGB;
		type = GL_UNSIGNED_BYTE;
		break;

	case 32:
		internal_format = GL_RGBA;
		format = GL_BGRA_EXT;
		type = GL_UNSIGNED_INT_8_8_8_8_REV;
		break;

	default:
		return;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, data);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

	if (mipMaps)
		gluBuild2DMipmaps(GL_TEXTURE_2D, internal_format, width, height, format, type, data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

unsigned short Texture::getWidth()
{
	return width;
}

unsigned short Texture::getHeight()
{
	return height;
}

unsigned short Texture::getDepth()
{
	return depth;
}

bool Texture::getMipMaps()
{
	return mipMaps;
}

void Texture::setTextureFilter(int textureFilter)
{
	this->textureFilter = textureFilter;
}

int Texture::getTextureFilter()
{
	return textureFilter;
}

void Texture::enable()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, id);

	// no texture filter
	if (textureFilter == NO_FILTER)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (mipMaps)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	// bilinear filter
	if (textureFilter == BILINEAR_FILTER)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (mipMaps)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	// trilinear filter
	else if (textureFilter == TRILINEAR_FILTER)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (mipMaps)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	// anisotropic filter
	else if (textureFilter == ANISOTROPIC_FILTER)
	{
		if (ExtensionManager::anisotropyAvailable())
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, ExtensionManager::getMaxAnisotropy());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (mipMaps)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}
