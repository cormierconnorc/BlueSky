/*
 * SlaveCamera.cpp
 *
 *  Created on: Jul 21, 2013
 *      Author: Connor Cormier
 */

#include "SlaveCamera.h"

SlaveCamera::SlaveCamera(float fov, int width, int height) : Camera(fov, width, height) {}
SlaveCamera::SlaveCamera(float fov, int width, int height, glm::vec3 pos, glm::vec3 rot) : Camera(fov, width, height, pos, rot) {}

void SlaveCamera::update()
{
	//Position and orientation are tied to the action of another object, not touched here.
	updateTime();

	updateMatrices();
}


