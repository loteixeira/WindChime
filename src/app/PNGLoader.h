// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com
//
// This class is based on PNGLoader class from IrrlichtEngine (http://irrlicht.sourceforge.net/)


#ifndef PNGLOADER_H_
#define PNGLOADER_H_

#include "../Config.h"
#include "Texture.h"

class PNGLoader
{
public:
	static Texture* load(string fileName, bool mipMaps);
};

#endif /* PNGLOADER_H_ */
