// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "RenderObject.h"
#include "RenderUtil.h"

RenderObject::RenderObject(const Vector3& pivot, const Vector3& position, float mass) : Particle(pivot, position, mass)
{
}

RenderObject::~RenderObject()
{

}


// Fast Point-In-Cylinder Collision Test
// Thanks to Greg James, he wrote a great tutorial about! :D
// Source: http://www.flipcode.com/archives/Fast_Point-In-Cylinder_Test.shtml
bool RenderObject::pointToCylinderCollision(const Vector3& point, float* distance)
{
	Vector3 cylinderVector(getHitEnd() - getHitStart());
	Vector3 testVector(point - getHitStart());
	float dot = testVector.dotProduct(cylinderVector);
	float length_sq = pow(cylinderVector.length(), 2);
	float radius_sq = pow(getHitRadius(), 2);

	if (dot < 0.0f || dot > length_sq)
	{
		return false;
	}
	else
	{
		float dsq = (testVector.x * testVector.x + testVector.y * testVector.y + testVector.z * testVector.z) - (dot * dot) / length_sq;

		if (dsq > radius_sq)
		{
			return false;
		}
		else
		{
			*distance = sqrt(dsq);
			return true;
		}
	}
}

void RenderObject::movePosition(const Vector3& offset)
{
	Vector3 pos(getPosition());
	pos -= offset;
	pos.normalize();
	pos *= getLength();
	setPosition(pos);
}

void RenderObject::update(unsigned long timeSinceLastUpdate, Vector3 force)
{
	Particle::update(timeSinceLastUpdate, force);
}
