// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef CONEOBJECT_H_
#define CONEOBJECT_H_

#include "../Config.h"
#include "RenderObject.h"
#include "ShaderProgram.h"
#include "Vertex.h"

class ConeObject : public RenderObject
{
public:
	ConeObject(const Vector3& pivot, const Vector3& position, float mass, float coneLength, float coneRadius);
	virtual ~ConeObject();

	float getHitRadius();
	Vector3 getHitStart();
	Vector3 getHitEnd();

protected:
	virtual void render();

private:
	float coneLength, coneRadius;

	Vertex* vertexBuffer;
	unsigned long* indexBuffer;
	unsigned short vertexCount, indexCount;

	ShaderProgram* dirLightPerPixelShader;
};

#endif /* CONEOBJECT_H_ */
