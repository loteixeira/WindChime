// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef BODY_H_
#define BODY_H_

#include "../Config.h"
#include "Material.h"
#include "RenderObject.h"
#include "Vector3.h"
#include "Vertex.h"

class Body
{
public:
	Body(unsigned short count, const RenderObject** objects);
	~Body();

	float getMass();

	void setTranslation(const Vector3& translation);
	Vector3 getTranslation();

	void setChild(Body* child);
	Body* getChild();

	void render();
	void update(unsigned long timeSinceLastUpdate, Vector3 force);

private:
	vector<RenderObject*> renderObjects;
	Vector3 translation;
	Body* child;
};

#endif /* BODY_H_ */
