// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "CylinderObject.h"
#include "ExtensionManager.h"
#include "MeshFactory.h"
#include "RenderUtil.h"

CylinderObject::CylinderObject(const Vector3& pivot, const Vector3& position, float mass, float radius) : RenderObject(pivot, position, mass)
{
	this->radius = radius;

	MeshFactory::getCylinderMesh(&vertexBuffer, &vertexCount, &indexBuffer, &indexCount);
	dirLightPerPixelShader = MeshFactory::getDirLightPerPixelShader();
}

CylinderObject::~CylinderObject()
{

}

float CylinderObject::getHitRadius()
{
	return radius;
}

Vector3 CylinderObject::getHitStart()
{
	return getPivot();
}

Vector3 CylinderObject::getHitEnd()
{
	return getPosition() + getPivot();
}

void CylinderObject::render()
{
	RenderUtil::setMaterial(MeshFactory::getCylinderMaterial());

	Vector3 position = getPosition();
	float x_angle = pi - atan2(position.x, position.y);
	float z_angle = pi + atan2(position.z, position.y);

	glPushMatrix();

	glRotatef(z_angle * radianToDeegree, 1.0f, 0.0f, 0.0f);
	glRotatef(x_angle * radianToDeegree, 0.0f, 0.0f, 1.0f);
	glScalef(radius, getLength(), radius);

	if (ExtensionManager::shadersAvailable())
		dirLightPerPixelShader->enable();

	RenderUtil::render(vertexBuffer, vertexCount, indexBuffer, indexCount);

	if (ExtensionManager::shadersAvailable())
		ShaderProgram::disable();

	glPopMatrix();

	RenderUtil::unsetMaterial();
}
