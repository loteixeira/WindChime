// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com
//
// This class is based on Texture class from IrrlichtEngine (http://irrlicht.sourceforge.net/)


#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../Config.h"

class Texture
{
public:
	static const int NO_FILTER = 0;
	static const int BILINEAR_FILTER = 1;
	static const int TRILINEAR_FILTER = 2;
	static const int ANISOTROPIC_FILTER = 3;

	static void disable();

	Texture(void* data, unsigned short width, unsigned short height, unsigned short depth, bool mipMaps);
	~Texture();

	unsigned short getWidth();
	unsigned short getHeight();
	unsigned short getDepth();
	bool getMipMaps();

	void setTextureFilter(int textureFilter);
	int getTextureFilter();

	void enable();

private:
	unsigned short width, height, depth;
	bool mipMaps;

	int textureFilter;

	GLuint id;
};

#endif /* TEXTURE_H_ */
