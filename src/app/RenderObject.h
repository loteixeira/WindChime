// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef RENDEROBJECT_H_
#define RENDEROBJECT_H_

#include "../Config.h"
#include "Material.h"
#include "Particle.h"
#include "Vector3.h"

class RenderObject : public Particle
{
public:
	static const unsigned short cylinder_collision_vertices = 16;

	RenderObject(const Vector3& pivot, const Vector3& position, float mass);
	virtual ~RenderObject();

	bool pointToCylinderCollision(const Vector3& point, float* distance);
	void movePosition(const Vector3& offset);

	virtual float getHitRadius()=0;
	virtual Vector3 getHitStart()=0;
	virtual Vector3 getHitEnd()=0;

	virtual void render()=0;
	virtual void update(unsigned long timeSinceLastUpdate, Vector3 force);
};

#endif /* RENDEROBJECT_H_ */
