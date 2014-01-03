/*
 * BlueSky.cpp
 *
 *  Created on: Jul 15, 2013
 *      Author: Connor Cormier
 */

#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <list>
#include <string>
#include <sstream>
#include <time.h>

#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "SlaveCamera.h"
#include "ObjLoader.h"
#include "Entity.h"
#include "Sopwith.h"
#include "PlayerSopwith.h"
#include "Bullet.h"
#include "Terrain.h"


using namespace std;

const GLuint WIDTH = 700, HEIGHT = 700;
const string title("Blue Sky");
Camera* cam = new SlaveCamera(90.0f, WIDTH, HEIGHT);
Terrain* t;	//Do not initialize
list<Entity*> entities;

Camera* getCam()
{
	return cam;
}

Terrain* getTerrain()
{
	return t;
}

void addEntity(Entity* e)
{
	entities.push_back(e);
}

list<Entity*> getEntityList()
{
	return entities;
}

int initGL()
{
	if (!glfwInit())
	{
		cout << "Failed to initialize GLFW" << endl;
		return 1;
	}

	//Now set up glfw settings before opening window
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);	//AA
	//Don't do the following. It breaks things.
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);		//Set version 3
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);		//Set to 3.3
	//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		//Disable old OpenGL versions

	if (!glfwOpenWindow(WIDTH, HEIGHT, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
	{
		cout << "Failed to open window" << endl;
		return 1;
	}

	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW" << endl;
		return 1;
	}

	//Set up VAO
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	stringstream s;
	s << title << " ... Initializing" << endl;
	glfwSetWindowTitle(s.str().c_str());

	glfwDisable(GLFW_MOUSE_CURSOR);
	glfwEnable(GLFW_STICKY_KEYS);

	return 0;
}

time_t lastTime;
int fps = 0, ticks = 0;

void updateFPS()
{
	if (time(0) - lastTime >= 1)
	{
		lastTime = time(0);
		fps = ticks;
		ticks = 0;
	}
	else
		ticks++;

	stringstream s;
	s << title << ", FPS = " << fps;
	glfwSetWindowTitle(s.str().c_str());
}

void genSopwith(Camera* cam)
{
	srand(time(NULL));
	//Generate outside of a box 150.0f around the player
	glm::vec3 pos = cam->getPos();

	int min = 50, max = 100;

	float xOff, yOff, zOff;
	xOff = (rand() % ((max - min) * 2 + 1) - (max - min));
	yOff = (rand() % ((max - min) * 2 + 1) - (max - min));
	zOff = (rand() % ((max - min) * 2 + 1) - (max - min));

	xOff += (xOff > 0 ? min : -min);
	yOff += (yOff > 0 ? min : -min);
	zOff += (zOff > 0 ? min : -min);

	addEntity(new Sopwith(glm::vec3(pos.x + xOff, pos.y + yOff, pos.z + zOff)));
}

//Primary run function
void runProgram()
{
	t = new Terrain();

	t->genTerrain(glm::vec3(0, 0, 0), 500.0f);

	//Load shaders
	//GLuint programId = loadShaders("vertexShader.glsl", "fragmentShader.glsl");

	//Starting with 3 Sopwiths to render
	//NOTE: OBJECT SLICING OCCURS HERE. THEY'RE COPIED INTO ENTITY OBJECTS, NOT ADDED TO THE VECTOR!!! To fix, use pointers.
	//Setting an Entity object equal to a Sopwith tries to copy, resulting in splicing. Using the pointer allows a Sopwith to
	//Be treated as an entity.
	//Quats definitely take radians
	Entity* player = new PlayerSopwith(cam);
	entities.push_back(player);

	genSopwith(cam);

	//Set background color
	glClearColor(.52941f, .80784f, .98037f, 0);

	//Enable depth test
	glEnable(GL_DEPTH_TEST);

	//Accept fragment closer to camera than former fragment
	glDepthFunc(GL_LESS);

	do
	{
		//Update FPS in window title
		updateFPS();

		//Reset background color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update the camera
		cam->update();

		t->update(cam);

		//Render the Entities
		for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
		{
			bool del = (*it)->update();
			(*it)->render(cam);

			if (del)
			{
				//Add new Sopwith if a Sopwith is being destroyed
				if (dynamic_cast<Sopwith*>(*it))
					genSopwith(cam);

				delete * it;
				it = entities.erase(it);
			}
		}

		glfwSwapBuffers();

		//Windows thread sleep
		Sleep(15);
	} while ( glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));

}

int main()
{
	//Start out by initializing opengl
	if (initGL() != 0)
		return 1;

	runProgram();

	return 0;
}


