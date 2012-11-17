// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef HAMMEROBJECT_H_
#define HAMMEROBJECT_H_

#include "../Config.h"
#include "Material.h"
#include "MeshFactory.h"
#include "RenderObject.h"
#include "ShaderProgram.h"
#include "Vertex.h"

class HammerObject : public RenderObject
{
public:
	HammerObject(const Vector3& pivot, const Vector3& position, float mass, float hammerLength, float hammerRadius);
	virtual ~HammerObject();

	float getHitRadius();
	Vector3 getHitStart();
	Vector3 getHitEnd();

protected:
	virtual void render();

private:
	float hammerLength, hammerRadius;

	Vertex* vertexBuffer;
	unsigned long* indexBuffer;
	unsigned short vertexCount, indexCount;

	ShaderProgram* dirLightPerPixelShader;
};

#endif /* HAMMEROBJECT_H_ */
