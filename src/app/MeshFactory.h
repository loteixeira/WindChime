// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef MESHFACTORY_H_
#define MESHFACTORY_H_

#include "Material.h"
#include "ShaderProgram.h"
#include "Vertex.h"

class MeshFactory
{
public:
	static void createCylinderMesh(double cylinderVertexAngle);
	static void destroyCylinderMesh();
	static void getCylinderMesh(Vertex** vertexBuffer, unsigned short* vertexCount, unsigned long** indexBuffer, unsigned short* indexCount);

	static void createConeMesh(double coneVertexAngle);
	static void destroyConeMesh();
	static void getConeMesh(Vertex** vertexBuffer, unsigned short* vertexCount, unsigned long** indexBuffer, unsigned short* indexCount);

	static void createCylinderMaterial();
	static Material& getCylinderMaterial();
	static void createConeMaterial();
	static Material& getConeMaterial();
	static void createHammerMaterial();
	static Material& getHammerMaterial();

	static void createShaders();
	static void destroyShaders();
	static ShaderProgram* getDirLightPerPixelShader();
};

#endif /* MESHFACTORY_H_ */
