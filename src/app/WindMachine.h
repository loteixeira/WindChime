// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef WINDMACHINE_H_
#define WINDMACHINE_H_

#include "Vector3.h"

class WindMachine
{
public:
	WindMachine();
	~WindMachine();

	Vector3 getIntensity();

	void update();

private:
	static const unsigned long fixed_time = 1000;
	static const unsigned long variable_time = 2000;

	unsigned long lastUpdate, nextWind;
	Vector3 intensity;

	unsigned long getNextInterval();
	Vector3 getNextIntensity();
};

#endif /* WINDMACHINE_H_ */
