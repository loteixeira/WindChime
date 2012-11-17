// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "MeshFactory.h"
#include "ExtensionManager.h"
#include "Vector3.h"
#include <math.h>


//
// cylinder mesh methods
//
static Vertex* cylinderVertexBuffer;
static unsigned short cylinderVertexCount;
static unsigned long* cylinderIndexBuffer;
static unsigned short cylinderIndexCount;

void MeshFactory::createCylinderMesh(double cylinderVertexAngle)
{
	unsigned short counter, halfCount;

	// create vertex buffer
	cylinderVertexCount = (unsigned short) (2.0 * pi / cylinderVertexAngle);
	cylinderVertexCount *= 2;

	cylinderVertexBuffer = new Vertex[cylinderVertexCount];

	counter = 0;
	halfCount = cylinderVertexCount / 2;

	for (double theta = 0.0f; theta < 2.0 * pi; theta += cylinderVertexAngle)
	{
		float x = cos(theta);
		float z = sin(theta);

		Vector3 upperPosition(x, 0.0f, z), lowerPosition(x, -1.0f, z);
		Vector3 upperNormal(upperPosition), lowerNormal(lowerPosition);
		upperNormal.y = 1.0f;
		upperNormal.normalize();
		lowerNormal.normalize();

		cylinderVertexBuffer[counter].nx = upperNormal.x;
		cylinderVertexBuffer[counter].ny = upperNormal.y;
		cylinderVertexBuffer[counter].nz = upperNormal.z;
		cylinderVertexBuffer[counter].x = upperPosition.x;
		cylinderVertexBuffer[counter].y = upperPosition.y;
		cylinderVertexBuffer[counter].z = upperPosition.z;

		cylinderVertexBuffer[halfCount + counter].nx = lowerNormal.x;
		cylinderVertexBuffer[halfCount + counter].ny = lowerNormal.y;
		cylinderVertexBuffer[halfCount + counter].nz = lowerNormal.z;
		cylinderVertexBuffer[halfCount + counter].x = lowerPosition.x;
		cylinderVertexBuffer[halfCount + counter].y = lowerPosition.y;
		cylinderVertexBuffer[halfCount + counter].z = lowerPosition.z;

		counter++;
	}

	// create index buffer
	cylinderIndexCount = (cylinderVertexCount - 4) * 3 + cylinderVertexCount * 3;
	cylinderIndexBuffer = new unsigned long[cylinderIndexCount];

	counter = 0;
	unsigned short lastVertex = (cylinderVertexCount / 2) - 1;

	for (unsigned short i = 0; i < (cylinderVertexCount / 2) - 2; i++)
	{
		cylinderIndexBuffer[counter++] = i;
		cylinderIndexBuffer[counter++] = i + 1;
		cylinderIndexBuffer[counter++] = lastVertex;

		cylinderIndexBuffer[counter++] = halfCount + i;
		cylinderIndexBuffer[counter++] = halfCount + i + 1;
		cylinderIndexBuffer[counter++] = halfCount + lastVertex;
	}

	for (unsigned short i = 0; i < (cylinderVertexCount / 2); i++)
	{
		if (i == 0)
		{
			cylinderIndexBuffer[counter++] = lastVertex;
			cylinderIndexBuffer[counter++] = i;
			cylinderIndexBuffer[counter++] = halfCount + lastVertex;

			cylinderIndexBuffer[counter++] = i;
			cylinderIndexBuffer[counter++] = halfCount + i;
			cylinderIndexBuffer[counter++] = halfCount + lastVertex;
		}
		else
		{
			cylinderIndexBuffer[counter++] = i - 1;
			cylinderIndexBuffer[counter++] = i;
			cylinderIndexBuffer[counter++] = halfCount + i - 1;

			cylinderIndexBuffer[counter++] = i;
			cylinderIndexBuffer[counter++] = halfCount + i;
			cylinderIndexBuffer[counter++] = halfCount + i - 1;
		}
	}

	cout << "Cylinder Mesh: vertices = " << cylinderIndexCount << ", faces = " << cylinderIndexCount / 3 << endl;
}

void MeshFactory::destroyCylinderMesh()
{
	delete cylinderVertexBuffer;
	delete cylinderIndexBuffer;
}

void MeshFactory::getCylinderMesh(Vertex** vertexBuffer, unsigned short* vertexCount, unsigned long** indexBuffer, unsigned short* indexCount)
{
	*vertexBuffer = cylinderVertexBuffer;
	*vertexCount = cylinderVertexCount;
	*indexBuffer = cylinderIndexBuffer;
	*indexCount = cylinderIndexCount;
}


//
// cone mesh methods
//
static Vertex* coneVertexBuffer;
static unsigned short coneVertexCount;
static unsigned long* coneIndexBuffer;
static unsigned short coneIndexCount;

void MeshFactory::createConeMesh(double coneVertexAngle)
{
	// vertex buffer
	coneVertexCount = (unsigned short) ((2.0 * pi) / coneVertexAngle) + 1;
	coneVertexBuffer = new Vertex[coneVertexCount];

	Vector3 peakPosition(zero), peakNormal(0.0f, 1.0f, 0.0f);
	coneVertexBuffer[0].nx = peakNormal.x;
	coneVertexBuffer[0].ny = peakNormal.y;
	coneVertexBuffer[0].nz = peakNormal.z;
	coneVertexBuffer[0].x = peakPosition.x;
	coneVertexBuffer[0].y = peakPosition.y;
	coneVertexBuffer[0].z = peakPosition.z;

	unsigned short count = 1;

	for (double theta = 0.0f; theta < 2.0 * pi; theta += coneVertexAngle)
	{
		float x = cos(theta), z = sin(theta);

		Vector3 position(x, -1.0f, z);
		Vector3 normal(x, 0.0f, z);

		coneVertexBuffer[count].nx = normal.x;
		coneVertexBuffer[count].ny = normal.y;
		coneVertexBuffer[count].nz = normal.z;
		coneVertexBuffer[count].x = position.x;
		coneVertexBuffer[count].y = position.y;
		coneVertexBuffer[count].z = position.z;

		count++;
	}

	// index buffer
	coneIndexCount = (coneVertexCount - 1) * 3;
	coneIndexBuffer = new unsigned long[coneIndexCount];

	count = 0;
	for (unsigned short i = 1; i < coneVertexCount; i++)
	{
		if (i == 1)
		{
			coneIndexBuffer[count++] = coneVertexCount - 1;
			coneIndexBuffer[count++] = 0;
			coneIndexBuffer[count++] = i;
		}
		else
		{
			coneIndexBuffer[count++] = i - 1;
			coneIndexBuffer[count++] = 0;
			coneIndexBuffer[count++] = i;
		}
	}

	cout << "Cone Mesh: vertices = " << coneIndexCount << ", faces = " << coneIndexCount / 3 << endl;
}

void MeshFactory::destroyConeMesh()
{
	delete coneVertexBuffer;
	delete coneIndexBuffer;
}

void MeshFactory::getConeMesh(Vertex** vertexBuffer, unsigned short* vertexCount, unsigned long** indexBuffer, unsigned short* indexCount)
{
	*vertexBuffer = coneVertexBuffer;
	*vertexCount = coneVertexCount;
	*indexBuffer = coneIndexBuffer;
	*indexCount = coneIndexCount;
}


//
// material methods
//
static Material cylinderMaterial, coneMaterial, hammerMaterial;

void MeshFactory::createCylinderMaterial()
{
	cylinderMaterial.lighting = true;
	cylinderMaterial.wireframe = false;

	cylinderMaterial.ambientColor[0] = 0.7f;
	cylinderMaterial.ambientColor[1] = 0.7f;
	cylinderMaterial.ambientColor[2] = 0.7f;
	cylinderMaterial.ambientColor[3] = 1.0f;

	cylinderMaterial.diffuseColor[0] = 0.5f;
	cylinderMaterial.diffuseColor[1] = 0.5f;
	cylinderMaterial.diffuseColor[2] = 0.5f;
	cylinderMaterial.diffuseColor[3] = 1.0f;

	cylinderMaterial.specularColor[0] = 1.0f;
	cylinderMaterial.specularColor[1] = 1.0f;
	cylinderMaterial.specularColor[2] = 1.0f;
	cylinderMaterial.specularColor[3] = 1.0f;

	cylinderMaterial.shininess = 15;
}

Material& MeshFactory::getCylinderMaterial()
{
	return cylinderMaterial;
}

void MeshFactory::createConeMaterial()
{
	coneMaterial.lighting = true;
	coneMaterial.wireframe = false;

	coneMaterial.ambientColor[0] = 0.6f;
	coneMaterial.ambientColor[1] = 0.6f;
	coneMaterial.ambientColor[2] = 0.6f;
	coneMaterial.ambientColor[3] = 1.0f;

	coneMaterial.diffuseColor[0] = 0.6f;
	coneMaterial.diffuseColor[1] = 0.6f;
	coneMaterial.diffuseColor[2] = 0.6f;
	coneMaterial.diffuseColor[3] = 1.0f;

	coneMaterial.specularColor[0] = 0.3f;
	coneMaterial.specularColor[1] = 0.3f;
	coneMaterial.specularColor[2] = 0.3f;
	coneMaterial.specularColor[3] = 1.0f;

	coneMaterial.shininess = 10;
}

Material& MeshFactory::getConeMaterial()
{
	return coneMaterial;
}

void MeshFactory::createHammerMaterial()
{
	hammerMaterial.lighting = true;
	hammerMaterial.wireframe = false;

	hammerMaterial.ambientColor[0] = 0.4f;
	hammerMaterial.ambientColor[1] = 0.4f;
	hammerMaterial.ambientColor[2] = 0.4f;
	hammerMaterial.ambientColor[3] = 1.0f;

	hammerMaterial.diffuseColor[0] = 0.7f;
	hammerMaterial.diffuseColor[1] = 0.7f;
	hammerMaterial.diffuseColor[2] = 0.7f;
	hammerMaterial.diffuseColor[3] = 1.0f;

	hammerMaterial.specularColor[0] = 1.0f;
	hammerMaterial.specularColor[1] = 1.0f;
	hammerMaterial.specularColor[2] = 1.0f;
	hammerMaterial.specularColor[3] = 1.0f;

	hammerMaterial.shininess = 30;
}

Material& MeshFactory::getHammerMaterial()
{
	return hammerMaterial;
}


//
// shaders methods
//
static ShaderProgram* dirLightPerPixelShader = NULL;

void MeshFactory::createShaders()
{
	if (ExtensionManager::shadersAvailable())
	{
		cout << "Loading shader program" << endl;

		dirLightPerPixelShader = new ShaderProgram();

		if (!dirLightPerPixelShader->addFragShader("shaders/DirLightPerPixelFrag.glsl"))
		{
			cout << "DirLightPerPixel fragment shader compilation error:" << endl;
			cout << dirLightPerPixelShader->getInfoLog() << endl;
		}

		if (!dirLightPerPixelShader->addVertexShader("shaders/DirLightPerPixelVert.glsl"))
		{
			cout << "DirLightPerPixel vertex shader compilation error:" << endl;
			cout << dirLightPerPixelShader->getInfoLog() << endl;
		}

		if (!dirLightPerPixelShader->link())
		{
			cout << "DirLightPerPixel program linking error:" << endl;
			cout << dirLightPerPixelShader->getInfoLog() << endl;
		}
	}
}

void MeshFactory::destroyShaders()
{
	delete dirLightPerPixelShader;
}

ShaderProgram* MeshFactory::getDirLightPerPixelShader()
{
	return dirLightPerPixelShader;
}
