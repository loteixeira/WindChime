// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "ConfigFile.h"

ConfigFile::ConfigFile(string fileName)
{
	FILE* file;
	char buffer[256];

	file = fopen(fileName.c_str(), "rt");

	while (!feof(file))
	{
		buffer[0] = 0;
		fgets(buffer, 256, file);

		unsigned short length = strlen(buffer);

		if (buffer[length - 1] == '\n' || buffer[length - 1] == '\r')
			buffer[length - 1] = 0;

		// comment
		if (buffer[0] == '#')
			continue;

		// window width
		else if (strncmp(buffer, "WindowWidth=", 12) == 0)
			windowWidth = atoi(&buffer[12]);
		// window height
		else if (strncmp(buffer, "WindowHeight=", 13) == 0)
			windowHeight = atoi(&buffer[13]);
		// window color depth
		else if (strncmp(buffer, "WindowColorDepth=", 17) == 0)
			windowColorDepth = atoi(&buffer[17]);
		// full screen window
		else if (strncmp(buffer, "FullScreenWindow=", 17) == 0)
		{
			if (strcmp(&buffer[17], "true") == 0)
				fullScreenWindow = true;
			else
				fullScreenWindow = false;
		}
		// texture filter
		else if (strncmp(buffer, "TextureFilter=", 14) == 0)
			textureFilter = atoi(&buffer[14]);
		// texture mip maps
		else if (strncmp(buffer, "TextureMipMaps=", 15) == 0)
		{
			if (strcmp(&buffer[15], "true") == 0)
				textureMipMaps = true;
			else
				textureMipMaps = false;
		}
		// force no shaders
		else if (strncmp(buffer, "ForceNoShaders=", 15) == 0)
		{
			if (strcmp(&buffer[15], "true") == 0)
				forceNoShaders = true;
			else
				forceNoShaders = false;
		}
		// multisample
		else if (strncmp(buffer, "Multisample=", 12) == 0)
			multisample = atoi(&buffer[12]);
	}

	fclose(file);
}

ConfigFile::~ConfigFile()
{

}

unsigned short ConfigFile::getWindowWidth()
{
	return windowWidth;
}

unsigned short ConfigFile::getWindowHeight()
{
	return windowHeight;
}

unsigned short ConfigFile::getWindowColorDepth()
{
	return windowColorDepth;
}

bool ConfigFile::getFullScreenWindow()
{
	return fullScreenWindow;
}

int ConfigFile::getTextureFilter()
{
	return textureFilter;
}

bool ConfigFile::getTextureMipMaps()
{
	return textureMipMaps;
}

bool ConfigFile::getForceNoShaders()
{
	return forceNoShaders;
}

int ConfigFile::getMultisample()
{
	return multisample;
}
