// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "Body.h"
#include "HammerObject.h"
#include "RenderUtil.h"

Body::Body(unsigned short count, const RenderObject** objects)
{
	for (unsigned short i = 0; i < count; i++)
		renderObjects.push_back((RenderObject*) objects[i]);

	child = NULL;
}

Body::~Body()
{
	vector<RenderObject*>::iterator it;

	for (it = renderObjects.begin(); it != renderObjects.end(); it++)
		delete (*it);

	if (child != NULL)
		delete child;
}

float Body::getMass()
{
	float total = 0.0f;
	vector<RenderObject*>::iterator it;

	for (it = renderObjects.begin(); it != renderObjects.end(); it++)
		total += (*it)->getMass();

	return total;

}

void Body::setTranslation(const Vector3& translation)
{
	this->translation = translation;
}

Vector3 Body::getTranslation()
{
	return translation;
}

void Body::setChild(Body* child)
{
	// remove old child mass
	if (this->child != NULL)
		renderObjects[0]->setMass(renderObjects[0]->getMass() - this->child->getMass());

	this->child = child;

	// add new child mass
	if (this->child != NULL)
		renderObjects[0]->setMass(renderObjects[0]->getMass() + this->child->getMass());
}

Body* Body::getChild()
{
	return child;
}

void Body::render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(translation.x, translation.y, translation.z);

	vector<RenderObject*>::iterator it;

	for (it = renderObjects.begin(); it != renderObjects.end(); it++)
	{
		Vector3 pivot((*it)->getPivot());

		glPushMatrix();
		glTranslatef(pivot.x, pivot.y, pivot.z);
		(*it)->render();
		glPopMatrix();
	}

	// the biggest bad-pattern ever!!
	if (child != NULL)
	{
		Vector3 childPosition(renderObjects[0]->getPivot() + renderObjects[0]->getPosition());

		glPushMatrix();
		glTranslatef(childPosition.x, childPosition.y, childPosition.z);
		child->render();
		glPopMatrix();
	}

	glPopMatrix();
}

void Body::update(unsigned long timeSinceLastUpdate, Vector3 force)
{
	vector<RenderObject*>::iterator it;

	for (it = renderObjects.begin(); it != renderObjects.end(); it++)
		(*it)->update(timeSinceLastUpdate, force);

	for (size_t i = 0; i < renderObjects.size(); i++)
	{
		for (size_t j = 0; j < renderObjects.size(); j++)
		{
			if (i != j)
			{
				Vector3 basePosition;
				Vector3 radialDirection(renderObjects[i]->getForceDirection());
				radialDirection *= renderObjects[i]->getHitRadius();

				float length = tan((2.0 * pi) / RenderObject::cylinder_collision_vertices) * radialDirection.length();
				unsigned char counter = 0, counterLimit;
				bool collision = false;

				if (dynamic_cast<HammerObject*>(renderObjects[i]) != NULL)
					counterLimit = 2;
				else
					counterLimit = 1;

				do
				{
					if (counter == 0)
					{
						basePosition = renderObjects[i]->getHitEnd();
						counter++;
					}
					else if (counter == 1)
					{
						basePosition = renderObjects[i]->getHitStart();
						counter++;
					}

					for (unsigned short k = 0; k < RenderObject::cylinder_collision_vertices; k++)
					{
						Vector3 circleDirection(basePosition.crossProduct(radialDirection));
						circleDirection.length(length);

						radialDirection += circleDirection;
						radialDirection.length(renderObjects[i]->getHitRadius());

						float distance;

						if (renderObjects[j]->pointToCylinderCollision(radialDirection + basePosition, &distance))
						{
							Vector3 offset(renderObjects[i]->getHitEnd() - renderObjects[j]->getHitEnd());
							renderObjects[j]->movePosition(offset.length(distance));

							Vector3 i_momentum(renderObjects[i]->getMomentum()), j_momentum(renderObjects[j]->getMomentum());
							renderObjects[i]->setMomentum(j_momentum);
							renderObjects[j]->setMomentum(i_momentum);

							collision = true;
							break;
						}
					}
				}
				while(counter < counterLimit && !collision);
			}
		}

		if (child != NULL)
			child->update(timeSinceLastUpdate, force);
	}
}
