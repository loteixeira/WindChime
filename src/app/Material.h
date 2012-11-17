// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../Config.h"

class Material
{
public:
	bool depth;
	bool lighting;
	bool wireframe;

	float ambientColor[4];
	float diffuseColor[4];
	float specularColor[4];

	unsigned char shininess;

	Material()
	{
		depth = true;
		lighting = true;
		wireframe = false;

		ambientColor[0] = 1.0f;
		ambientColor[1] = 1.0f;
		ambientColor[2] = 1.0f;
		ambientColor[3] = 1.0f;

		diffuseColor[0] = 1.0f;
		diffuseColor[1] = 1.0f;
		diffuseColor[2] = 1.0f;
		diffuseColor[3] = 1.0f;

		specularColor[0] = 0.0f;
		specularColor[1] = 0.0f;
		specularColor[2] = 0.0f;
		specularColor[3] = 1.0f;

		shininess = 128;
	}

	Material(const Material& other)
	{
		depth = other.depth;
		lighting = other.lighting;
		wireframe = other.wireframe;

		memcpy(ambientColor, other.ambientColor, sizeof(float) * 4);
		memcpy(diffuseColor, other.diffuseColor, sizeof(float) * 4);
		memcpy(specularColor, other.specularColor, sizeof(float) * 4);

		shininess = other.shininess;
	}

	Material operator=(const Material& other)
	{
		depth = other.depth;
		lighting = other.lighting;
		wireframe = other.wireframe;

		memcpy(ambientColor, other.ambientColor, sizeof(float) * 4);
		memcpy(diffuseColor, other.diffuseColor, sizeof(float) * 4);
		memcpy(specularColor, other.specularColor, sizeof(float) * 4);

		shininess = other.shininess;

		return *this;
	}
};

#endif /* MATERIAL_H_ */
