// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include "../Config.h"

class ConfigFile
{
public:
	ConfigFile(string fileName);
	~ConfigFile();

	unsigned short getWindowWidth();
	unsigned short getWindowHeight();
	unsigned short getWindowColorDepth();
	bool getFullScreenWindow();
	int getTextureFilter();
	bool getTextureMipMaps();
	bool getForceNoShaders();
	int getMultisample();

private:
	unsigned short windowWidth, windowHeight, windowColorDepth;
	bool fullScreenWindow;
	int textureFilter;
	bool textureMipMaps;
	bool forceNoShaders;
	int multisample;
};

#endif /* CONFIGFILE_H_ */
