// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef RENDERUTIL_H_
#define RENDERUTIL_H_

#include "Material.h"
#include "Texture.h"
#include "Vector3.h"
#include "Vertex.h"

class RenderUtil
{
public:
	static void setMaterial(const Material& material);
	static void unsetMaterial();

	static void renderVector(const Vector3& vector, const Vector3& pivot = zero);
	static void renderLine(const Vector3& start, const Vector3& end);
	static void renderTexture(Texture* tex, const Vector3& leftTop, const Vector3& rightBottom);
	static void render(Vertex* vertexBuffer, unsigned short vertexCount, unsigned long* indexBuffer, unsigned short indexCount);
};

#endif /* RENDERUTIL_H_ */
