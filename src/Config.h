// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef CONFIG_H_
#define CONFIG_H_


//
// Libraries
//
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;


//
// Operational system stuff
//
#ifdef WIN32
#	define WIN32_ENV
#	include <windows.h>
#	include <mmsystem.h>
#	include "wglext.h"
#else
#	error Only for windows!
#endif /* WIN32 */


//
// Application macros
//
#define CONFIG_FILE_NAME "WindChime.cfg"


//
// Global variables
//
static const double pi = 3.1415926535897932384626433832795;
static const double deegreeToRadian = pi / 180.0;
static const double radianToDeegree = 180.0 / pi;


//
// Global functions
//
unsigned long getTime();

#endif /* CONFIG_H_ */
