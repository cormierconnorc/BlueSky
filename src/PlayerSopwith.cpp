/*
 * PlayerSopwith.cpp
 *
 *  Created on: Jul 21, 2013
 *      Author: Connor Cormier
 */

#include "PlayerSopwith.h"
#include <iostream>

using namespace std;

constexpr float PlayerSopwith::UP_FACTOR, PlayerSopwith::FOR_FACTOR;

PlayerSopwith::PlayerSopwith(Camera* cam) : Sopwith(), cam(cam) {}
PlayerSopwith::PlayerSopwith(Camera* cam, const glm::quat& orientation) : Sopwith(orientation), cam(cam) {}
PlayerSopwith::PlayerSopwith(Camera* cam, const glm::vec3& pos) : Sopwith(pos), cam(cam) {}
PlayerSopwith::PlayerSopwith(Camera* cam, const glm::vec3& pos, const glm::quat& orientation) : Sopwith(pos, orientation), cam(cam) {}
PlayerSopwith::PlayerSopwith(Camera* cam, const glm::vec3& pos, const glm::vec3& rot) : Sopwith(pos, rot), cam(cam) {}

bool PlayerSopwith::update()
{
	//Perform necessary entity update operations
	if (Entity::update() && !checkGroundCollision())
		hitPoints--;
	else if (checkGroundCollision())
		return true;

	acceptInput();

	cam->setPos((getPos() + (getForward() * FOR_FACTOR) + (getUp() * UP_FACTOR)));
	cam->setOrientation(getQuaternion());

	//Handle local rotation of camera
	cam->rotateAbout(glm::quat(glm::vec3(camRotX, camRotY, 0.0f)));

	return hitPoints <= 0;
}

void PlayerSopwith::acceptInput()
{
	//Acceleration and deceleration controlled by 'w' and 's'
	if (glfwGetKey('W'))
		velocity += ACCEL;
	if (glfwGetKey('S'))
		velocity -= ACCEL;

	if (velocity > MAX_VELOCITY)
		velocity = MAX_VELOCITY;
	else if (velocity < MIN_VELOCITY)
		velocity = MIN_VELOCITY;

	//Now move forward
	moveForward(velocity * dT);

	//Slow rotation about the y axis controlled with 'a' and 'd' keys
	if (glfwGetKey('D'))
		rotateRelativeY(-MAX_ROT_SPEED * dT * Y_ROT_FACTOR);
	if (glfwGetKey('A'))
		rotateRelativeY(MAX_ROT_SPEED * dT * Y_ROT_FACTOR);

	//Mouse controls rotation about x and z axes
	int x, y;

	//Get mouse position
	glfwGetMousePos(&x, &y);

	//Reset mouse position
	glfwSetMousePos(cam->getWidth() / 2, cam->getHeight() / 2);

	//Get change in mouse position
	int dX = x - cam->getWidth() / 2;
	int dY = y - cam->getHeight() / 2;

	float rotX, rotZ;

	rotX = dY * MAX_ROT_SPEED * dT;
	rotZ = dX * MAX_ROT_SPEED * dT;

	if (!glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
	{
		//Restrict to max turn speed
		rotX = min(rotX, X_ROT_FACTOR * MAX_ROT_SPEED * dT);
		rotX = max(rotX, -X_ROT_FACTOR * MAX_ROT_SPEED * dT);
		rotZ = min(rotZ, Z_ROT_FACTOR * MAX_ROT_SPEED * dT);
		rotZ = max(rotZ, -Z_ROT_FACTOR * MAX_ROT_SPEED * dT);

		//Execute local rotations
		rotateRelativeX(rotX);
		rotateRelativeZ(rotZ);

		//Slowly rotate the camera back towards the 0 position
		if (abs(camRotX) < .01f)
			camRotX = 0.0f;
		else
			camRotX += (camRotX > 0 ? -.01f : .01f);
		if (abs(camRotY) < .01f)
			camRotY = 0.0f;
		else
			camRotY += (camRotY > 0 ? -.01f : .01f);
	}
	else
	{
		//If the mouse button is down, add the rotation to cam rot
		camRotX += rotX;
		//Inverted for more natural controls
		camRotY -= rotZ;

		//Set to reasonable maxes
		camRotX = min(camRotX, MAX_X_ROT);
		camRotX = max(camRotX, -MAX_X_ROT);
		camRotY = min(camRotY, MAX_Y_ROT);
		camRotY = max(camRotY, -MAX_Y_ROT);
	}

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
		fireBullet();
}


