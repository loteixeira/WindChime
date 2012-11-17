// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "Particle.h"
#include "RenderUtil.h"

Particle::Particle(const Vector3& pivot, const Vector3& position, float mass)
{
	this->pivot = pivot;
	this->position = position;
	this->mass = mass;

	length = position.length();
	gravityForce = gravity * mass;
}

Particle::~Particle()
{

}

void Particle::setMass(float mass)
{
	this->mass = mass;
}

float Particle::getMass()
{
	return mass;
}

float Particle::getLength()
{
	return length;
}

Vector3 Particle::getPivot()
{
	return pivot;
}

Vector3 Particle::getPosition()
{
	return position;
}

Vector3 Particle::getGravityForce()
{
	return gravityForce;
}

Vector3 Particle::getForceDirection()
{
	return forceDirection;
}

Vector3 Particle::getVelocity()
{
	return velocity;
}

void Particle::setMomentum(const Vector3& momentum)
{
	velocity = momentum / mass;
}

Vector3 Particle::getMomentum()
{
	return velocity * mass;
}

void Particle::update(unsigned long timeSinceLastUpdate, Vector3 force)
{
	float elapsedTime = float(timeSinceLastUpdate) / 1000.0f;

	Vector3 resultForce(force + gravityForce);
	resultForce *= elapsedTime;
	forceDirection = position - (position + resultForce).normalize() * length;
	forceDirection.normalize();

	Vector3 forceComponent(forceDirection * forceDirection.dotProduct(resultForce));

	velocity = (forceDirection * velocity.dotProduct(forceDirection)) + (forceComponent / mass);
	velocity *= friction_constant;

	position += velocity;
	position.length(length);
}

void Particle::setPosition(const Vector3& position)
{
	this->position = position;
}
