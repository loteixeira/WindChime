// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "../Config.h"
#include "App.h"
#include "ConeObject.h"
#include "CylinderObject.h"
#include "ExtensionManager.h"
#include "HammerObject.h"
#include "PNGLoader.h"
#include "Material.h"
#include "MeshFactory.h"
#include "RenderUtil.h"
#include "Vector3.h"

#define RENDER_OBJECTS_NUM 6

Vector3 lightDirection;

App::App(ConfigFile* configFile)
{
	this->configFile = configFile;

	cout << "Wind Chime by Lucas Teixeira (loteixeira@gmail.com)" << endl;
	cout << "Window: " << configFile->getWindowWidth() << "x" << configFile->getWindowHeight() << "x" << configFile->getWindowColorDepth();
	cout << " (" << (configFile->getFullScreenWindow() ? "fullscreen" : "windowed") << " mode)" << endl;
	cout << "Video Card: " << glGetString(GL_VENDOR) << " " << glGetString(GL_RENDERER) << endl;
	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

	// Extensions initialization
	ExtensionManager::initialize();

	if (configFile->getForceNoShaders())
		ExtensionManager::disableShaders();

	// graphics initialization
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glViewport(0, 0, configFile->getWindowWidth(), configFile->getWindowHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPointSize(5.0f);

	// Mesh factory initialization
	MeshFactory::createCylinderMesh(pi / 16.0);
	MeshFactory::createConeMesh(pi / 16.0);
	MeshFactory::createCylinderMaterial();
	MeshFactory::createConeMaterial();
	MeshFactory::createHammerMaterial();
	MeshFactory::createShaders();

	// Background initialization
	cout << "Loading background image" << endl;
	background = PNGLoader::load("background.png", configFile->getTextureMipMaps());
	background->setTextureFilter(configFile->getTextureFilter());

	// Sunlight initialization
	lightDirection = Vector3(-0.75f, 0.1f, -0.3f);
	setupLight(lightDirection.normalize());

	// physics initialization
	const RenderObject* objects[RENDER_OBJECTS_NUM];
	objects[0] = new HammerObject(zero, Vector3(-0.006f, -0.3f, 0.006), 0.3f, 0.05f, 0.05f);

	for (unsigned char i = 0; i < RENDER_OBJECTS_NUM - 1; i++)
	{
		double angle = (2.0 * pi / double(RENDER_OBJECTS_NUM - 1)) * double(i);
		float length = -0.35f - (i * 0.08f);
		float mass = 0.15f + (i * 0.020f);
		Vector3 pivot(-0.085f * sin(angle), -0.003f, -0.085f * cos(angle));
		Vector3 position(float(rand() % 10) / 1000.0f, length - 0.003f, float(rand() % 10) / 1000.0f);
		objects[i + 1] = new CylinderObject(pivot, position, mass, 0.0175f);
	}

	Body* child = new Body(RENDER_OBJECTS_NUM, objects);

	const RenderObject* coneObject = new ConeObject(zero, Vector3(0.006f, -0.8f, -0.006), 0.3f, 0.1f, 0.115f);
	body = new Body(1, &coneObject);
	body->setChild(child);

	body->setTranslation(Vector3(0.2f, 1.3f, 0.0f));

	windMachine = new WindMachine();

	// time initialization
	lastUpdate = lastFrameUpdate = getTime();
	frameCounter = 0;
	lastFrameRate = 0;

	srand(time(NULL));

	// Let's go!
	cout << "Starting simulation" << endl;
}

App::~App()
{
	MeshFactory::destroyCylinderMesh();
	MeshFactory::destroyConeMesh();
	MeshFactory::destroyShaders();

	delete background;

	delete body;
	delete windMachine;
}

string App::getWindowCaption()
{
	char buffer[256];
	sprintf(buffer, "WindChime | Frames per Second: %u", lastFrameRate);
	return string(buffer);
}

void App::run()
{
	unsigned long currentUpdate = getTime();

	// frame rate counter
	frameCounter++;

	if (currentUpdate - lastFrameUpdate >= 1000)
	{
		lastFrameUpdate = currentUpdate;
		lastFrameRate = frameCounter;
		frameCounter = 0;
	}

	// rendering and physics update
	unsigned long timeSinceLastUpdate = currentUpdate - lastUpdate;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, configFile->getWindowWidth() / configFile->getWindowHeight(), 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	RenderUtil::renderTexture(background, Vector3(2.3f, 2.0f, 1.0f), Vector3(-2.3f, -2.0f, 1.0f));

	//renderGrid();
	body->render();

	windMachine->update();

	if (timeSinceLastUpdate > min_update_interval)
	{
		body->update(timeSinceLastUpdate, windMachine->getIntensity());
		lastUpdate = currentUpdate;
	}

	//RenderUtil::unsetMaterial();
	//RenderUtil::renderVector(lightDirection / 10.0f, Vector3(0.0f, -0.8f, 0.0f));
}

void App::setupLight(const Vector3& lightDirection)
{
	float info[4];

	// ambient component
	if (ExtensionManager::shadersAvailable())
	{
		info[0] = 0.3f;
		info[1] = 0.3f;
		info[2] = 0.3f;
		info[3] = 1.0f;
	}
	else
	{
		info[0] = 0.3f;
		info[1] = 0.3f;
		info[2] = 0.3f;
		info[3] = 1.0f;
	}
	glLightfv(GL_LIGHT0, GL_AMBIENT, info);

	// diffuse component
	if (ExtensionManager::shadersAvailable())
	{
		info[0] = 0.5f;
		info[1] = 0.5f;
		info[2] = 0.25f;
		info[3] = 1.0f;
	}
	else
	{
		info[0] = 0.02f;
		info[1] = 0.02f;
		info[2] = 0.01f;
		info[3] = 1.0f;
	}
	glLightfv(GL_LIGHT0, GL_DIFFUSE, info);

	// specular component
	if (ExtensionManager::shadersAvailable())
	{
		info[0] = 1.0f;
		info[1] = 1.0f;
		info[2] = 1.0f;
		info[3] = 1.0f;
	}
	else
	{
		info[0] = 0.0f;
		info[1] = 0.0f;
		info[2] = 0.0f;
		info[3] = 1.0f;
	}
	glLightfv(GL_LIGHT0, GL_SPECULAR, info);

	// light position
	info[0] = lightDirection.x;
	info[1] = lightDirection.y;
	info[2] = lightDirection.z;
	info[3] = 0.0f;
	glLightfv(GL_LIGHT0, GL_POSITION, info);

	// enable light
	glEnable(GL_LIGHT0);
}

void App::renderGrid()
{
	for (unsigned short i = 0; i < 10; i++)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glBegin(GL_LINES);
		glVertex3f(-1.0f, -0.8f, 1.0f - float(i) / 5.0f);
		glVertex3f(1.0f, -0.8f, 1.0f - float(i) / 5.0f);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(1.0f - float(i) / 5.0f, -0.8f, -1.0f);
		glVertex3f(1.0f - float(i) / 5.0f, -0.8f, 1.0f);
		glEnd();
	}

	glFlush();
}
