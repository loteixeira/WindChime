// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Vector3.h"

class Particle
{
public:
	Particle(const Vector3& pivot, const Vector3& position, float mass);
	virtual ~Particle();

	void setMass(float mass);
	float getMass();

	float getLength();
	Vector3 getPivot();
	Vector3 getPosition();
	Vector3 getGravityForce();
	Vector3 getForceDirection();
	Vector3 getVelocity();

	void setMomentum(const Vector3& momentum);
	Vector3 getMomentum();

	virtual void update(unsigned long timeSinceLastUpdate, Vector3 force);

protected:
	void setPosition(const Vector3& position);

private:
	static const float friction_constant = 0.995f;

	float length, mass;
	Vector3 pivot, position, gravityForce, forceDirection, velocity;
};

#endif /* PARTICLE_H_ */
