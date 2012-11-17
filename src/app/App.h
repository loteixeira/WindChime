// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef APP_H_
#define APP_H_

#include "Body.h"
#include "ConfigFile.h"
#include "Particle.h"
#include "Texture.h"
#include "Vector3.h"
#include "Vertex.h"
#include "WindMachine.h"

class App
{
public:
	App(ConfigFile* configFile);
	~App();

	string getWindowCaption();

	void run();

private:
	static const unsigned long min_update_interval = 15;

	ConfigFile* configFile;

	unsigned long lastUpdate, lastFrameUpdate;
	unsigned short frameCounter;
	unsigned short lastFrameRate;

	WindMachine* windMachine;
	Body* body;

	Texture* background;

	void setupLight(const Vector3& lightDirection);
	void renderGrid();
};

#endif /* APP_H_ */
