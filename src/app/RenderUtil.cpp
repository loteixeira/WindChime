// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "../Config.h"
#include "RenderUtil.h"

void RenderUtil::setMaterial(const Material& material)
{
	glDisable(GL_COLOR_MATERIAL);

	if (material.depth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if (material.lighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if (material.wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambientColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuseColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specularColor);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
}

void RenderUtil::unsetMaterial()
{
	glEnable(GL_COLOR_MATERIAL);
	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderUtil::renderVector(const Vector3& vector, const Vector3& pivot)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(pivot.x, pivot.y, pivot.z);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vector.x, vector.y, vector.z);
	glEnd();

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex3f(vector.x, vector.y, vector.z);
	glEnd();

	glPopMatrix();

	glFlush();
}

void RenderUtil::renderLine(const Vector3& start, const Vector3& end)
{
	glMatrixMode(GL_MODELVIEW);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(start.x, start.y, start.z);
	glVertex3f(end.x, end.y, end.z);
	glEnd();

	glFlush();
}

void RenderUtil::renderTexture(Texture* tex, const Vector3& leftTop, const Vector3& rightBottom)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	tex->enable();

	glBegin(GL_TRIANGLE_FAN);

	glVertex3f(leftTop.x, leftTop.y, leftTop.z);
	glTexCoord2f(1.0f, 0.0f);

	glVertex3f(rightBottom.x, leftTop.y, rightBottom.z);
	glTexCoord2f(1.0f, 1.0f);

	glVertex3f(rightBottom.x, rightBottom.y, rightBottom.z);
	glTexCoord2f(0.0f, 1.0f);

	glVertex3f(leftTop.x, rightBottom.y, leftTop.z);
	glTexCoord2f(0.0f, 0.0f);

	glEnd();

	glFlush();

	Texture::disable();
}

void RenderUtil::render(Vertex* vertexBuffer, unsigned short vertexCount, unsigned long* indexBuffer, unsigned short indexCount)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glNormalPointer(GL_FLOAT, sizeof(Vertex), &vertexBuffer[0].nx);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &vertexBuffer[0].red);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertexBuffer[0].u);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertexBuffer[0].x);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, indexBuffer);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glFlush();
}
