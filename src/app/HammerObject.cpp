// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "HammerObject.h"
#include "ExtensionManager.h"
#include "RenderUtil.h"

HammerObject::HammerObject(const Vector3& pivot, const Vector3& position, float mass, float hammerLength, float hammerRadius) : RenderObject(pivot, position, mass)
{
	this->hammerLength = hammerLength;
	this->hammerRadius = hammerRadius;

	MeshFactory::getCylinderMesh(&vertexBuffer, &vertexCount, &indexBuffer, &indexCount);
	dirLightPerPixelShader = MeshFactory::getDirLightPerPixelShader();
}

HammerObject::~HammerObject()
{

}

float HammerObject::getHitRadius()
{
	return hammerRadius;
}

Vector3 HammerObject::getHitStart()
{
	Vector3 hitStart(getPosition());
	hitStart.normalize();
	hitStart *= (getPosition().length() - hammerLength);
	return hitStart + getPivot();
}

Vector3 HammerObject::getHitEnd()
{
	Vector3 hitEnd(getPosition());
	hitEnd.normalize();
	hitEnd *= hammerLength;
	return getHitStart() + hitEnd + getPivot();
}

void HammerObject::render()
{
	Vector3 pivot = getPivot();
	Vector3 position = getPosition();
	Vector3 hammerPosition(position);
	hammerPosition.normalize();
	hammerPosition *= (position.length() - hammerLength);

	float x_angle = pi - atan2(position.x, position.y);
	float z_angle = pi + atan2(position.z, position.y);

	glPushMatrix();

	glTranslatef(pivot.x, pivot.y, pivot.z);

	RenderUtil::unsetMaterial();
	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(hammerPosition.x, hammerPosition.y, hammerPosition.z);
	glEnd();
	glFlush();

	glPushMatrix();

	glTranslatef(hammerPosition.x, hammerPosition.y, hammerPosition.z);
	glScalef(hammerRadius, hammerLength, hammerRadius);
	glRotatef(z_angle * radianToDeegree, 1.0f, 0.0f, 0.0f);
	glRotatef(x_angle * radianToDeegree, 0.0f, 0.0f, 1.0f);

	RenderUtil::setMaterial(MeshFactory::getHammerMaterial());

	if (ExtensionManager::shadersAvailable())
		dirLightPerPixelShader->enable();

	RenderUtil::render(vertexBuffer, vertexCount, indexBuffer, indexCount);

	if (ExtensionManager::shadersAvailable())
		ShaderProgram::disable();

	RenderUtil::unsetMaterial();

	glPopMatrix();
	glPopMatrix();
}
