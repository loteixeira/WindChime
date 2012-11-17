// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "../Config.h"
#include "RenderUtil.h"
#include "WindMachine.h"

WindMachine::WindMachine()
{
	lastUpdate = getTime();
	nextWind = getNextInterval();

	//cout << "Starting wind machine in " << (nextWind - lastUpdate) / 1000 << " second(s)." << endl;
}

WindMachine::~WindMachine()
{

}

Vector3 WindMachine::getIntensity()
{
	return intensity;
}

void WindMachine::update()
{
	unsigned long currentTime = getTime();

	if (currentTime >= nextWind)
	{
		nextWind = getNextInterval();

		// turn on
		if (rand() % 2 == 0)
		{
			intensity = getNextIntensity();
			//cout << "Wind machine on for " << (nextWind - currentTime) / 1000 << " second(s). (intensity = " << intensity.toString() << ")" << endl;
		}
		// turn off
		else
		{
			intensity = zero;
			//cout << "Wind machine off for " << (nextWind - currentTime) / 1000 << " second(s)." << endl;
		}
	}

	//RenderUtil::unsetMaterial();
	//RenderUtil::renderVector(intensity * 100.0f, Vector3(0.0f, -0.8f, 0.0f));
}

unsigned long WindMachine::getNextInterval()
{
	return getTime() + fixed_time + rand() % variable_time;
}

Vector3 WindMachine::getNextIntensity()
{
	Vector3 result;
	result.z = (float(rand() % 2500)) / 1000000.0f * -1.0f;
	result.y = 0.0f;
	result.x = (500.0f - float(rand() % 1000)) / 1000000.0f;

	return result;
}
