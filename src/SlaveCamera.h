/*
 * SlaveCamera.h
 *
 *  Created on: Jul 21, 2013
 *      Author: Connor Cormier
 */

#ifndef SLAVECAMERA_H_
#define SLAVECAMERA_H_

#include "Camera.h"

class SlaveCamera : public Camera
{
public:
	SlaveCamera(float, int, int);
	SlaveCamera(float, int, int, glm::vec3, glm::vec3);
	virtual void update();
};


#endif /* SLAVECAMERA_H_ */
