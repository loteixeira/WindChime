// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "ConeObject.h"
#include "ExtensionManager.h"
#include "MeshFactory.h"
#include "RenderUtil.h"

ConeObject::ConeObject(const Vector3& pivot, const Vector3& position, float mass, float coneLength, float coneRadius) : RenderObject(pivot, position, mass)
{
	this->coneLength = coneLength;
	this->coneRadius = coneRadius;

	MeshFactory::getConeMesh(&vertexBuffer, &vertexCount, &indexBuffer, &indexCount);
	dirLightPerPixelShader = MeshFactory::getDirLightPerPixelShader();
}

ConeObject::~ConeObject()
{

}

float ConeObject::getHitRadius()
{
	return coneRadius;
}

Vector3 ConeObject::getHitStart()
{
	Vector3 hitStart(getPosition());
	hitStart.normalize();
	hitStart *= (getPosition().length() - coneLength);
	return hitStart + getPivot();
}

Vector3 ConeObject::getHitEnd()
{
	Vector3 hitEnd(getPosition());
	hitEnd.normalize();
	hitEnd *= coneLength;
	return getHitStart() + hitEnd + getPivot();
}

void ConeObject::render()
{
	Vector3 pivot = getPivot();
	Vector3 position = getPosition();
	Vector3 conePosition(position);
	conePosition.normalize();
	conePosition *= (position.length() - coneLength);

	float x_angle = pi - atan2(position.x, position.y);
	float z_angle = pi + atan2(position.z, position.y);

	glPushMatrix();

	glTranslatef(pivot.x, pivot.y, pivot.z);

	RenderUtil::unsetMaterial();
	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(conePosition.x, conePosition.y, conePosition.z);
	glEnd();
	glFlush();

	glPushMatrix();

	glTranslatef(conePosition.x, conePosition.y, conePosition.z);
	glScalef(coneRadius, coneLength, coneRadius);
	glRotatef(z_angle * radianToDeegree, 1.0f, 0.0f, 0.0f);
	glRotatef(x_angle * radianToDeegree, 0.0f, 0.0f, 1.0f);

	RenderUtil::setMaterial(MeshFactory::getConeMaterial());

	if (ExtensionManager::shadersAvailable())
		dirLightPerPixelShader->enable();

	RenderUtil::render(vertexBuffer, vertexCount, indexBuffer, indexCount);

	if (ExtensionManager::shadersAvailable())
		ShaderProgram::disable();

	RenderUtil::unsetMaterial();

	glPopMatrix();
	glPopMatrix();
}
