/*
 * Camera.cpp
 *
 *  Created on: May 25, 2013
 *      Author: Connor Cormier
 */

#include "Camera.h"
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/gtx/transform.hpp>
#include <iostream>

using namespace std;

constexpr float Camera::ROT_SPEED, Camera::TRAN_SPEED, Camera::NEAR_POINT, Camera::FAR_POINT;

Camera::Camera(float fov, int width, int height)
{
	time = glfwGetTime();
	this->fov = fov;
	this->width = width;
	this->height = height;
	setPos(glm::vec3(0, 0, 0));
	setDir(glm::vec3(0, 0, 0));
}

Camera::Camera(float fov, int width, int height, glm::vec3 pos, glm::vec3 rot)
{
	time = glfwGetTime();
	this->fov = fov;
	this->width = width;
	this->height = height;
	setPos(pos);
	setDir(rot);
}

void Camera::update()
{
	updateTime();

	rotate();

	translate();

	updateMatrices();
}

int i = 0;

void Camera::rotate()
{
	int x, y;
	//Get current mouse position in window
	glfwGetMousePos(&x, &y);
	//Reset mouse to center of window
	glfwSetMousePos(width / 2, height / 2);

	//Get change in mouse position
	int dX = x - width / 2;
	int dY = y - height / 2;

	float dRotX, dRotY, dRotZ = 0;

	//Update rotations about x and y
	//Vertical angle, rotation about X
	dRotX = dY * dT * ROT_SPEED;
	//Horizontal angle, rotation about Y
	dRotY = -dX * dT * ROT_SPEED;

	if (glfwGetKey('Q'))
		dRotZ -= dT * ROT_SPEED;
	if (glfwGetKey('E'))
		dRotZ += dT * ROT_SPEED;

	//Quaternions? Quaternions
	orientation = orientation * glm::quat(glm::vec3(dRotX, dRotY, dRotZ));

	if (glfwGetKey('R'))
		resetOrientation();
}

//Rotate about absolute axis using rotation quat
void Camera::rotateAbout(const glm::quat& rotation)
{
	orientation = orientation * rotation;
	updateMatrices();
}

glm::vec3 Camera::rotateVector(const glm::mat4& rotationMatrix, const glm::vec3& vector)
{
	glm::vec4 result = rotationMatrix * glm::vec4(vector, 0.0f);
	glm::vec3 newVec = glm::vec3(result.x, result.y, result.z);
	newVec = glm::normalize(newVec);
	return newVec;
}

void Camera::resetOrientation()
{
	orientation = glm::quat(glm::vec3(0, 0, 0));
	updateMatrices();
}

void Camera::setOrientation(const glm::quat& orientation)
{
	this->orientation = orientation;
	updateMatrices();
}

void Camera::translate()
{
	//Normalize all vectors before performing calculations with them
	glm::normalize(dir);
	glm::normalize(right);
	glm::normalize(up);

	if (glfwGetKey('W'))
		pos += dir * (float)dT * TRAN_SPEED;
	if (glfwGetKey('S'))
		pos -= dir * (float)dT * TRAN_SPEED;
	if (glfwGetKey('D'))
		pos -= right * (float)dT * TRAN_SPEED;
	if (glfwGetKey('A'))
		pos += right * (float)dT * TRAN_SPEED;
	if (glfwGetKey(GLFW_KEY_LSHIFT))
		pos += up * (float)dT * TRAN_SPEED;
	if (glfwGetKey(GLFW_KEY_LCTRL))
		pos -= up * (float)dT * TRAN_SPEED;
}

void Camera::updateTime()
{
	dT = (glfwGetTime() - time);
	time = glfwGetTime();
}

void Camera::updateMatrices()
{
	//Update vectors using rotation matrix
	glm::mat4 rotationMatrix = glm::toMat4(orientation);
	dir = rotateVector(rotationMatrix, glm::vec3(0.0f, 0.0f, 1.0f));
	up = rotateVector(rotationMatrix, glm::vec3(0.0f, 1.0f, 0.0f));;
	right = rotateVector(rotationMatrix, glm::vec3(1.0f, 0.0f, 0.0f));;

	projectionMatrix = glm::perspective(fov, float(width) / height, NEAR_POINT, FAR_POINT);
	viewMatrix = glm::lookAt(pos, pos + dir, up);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

glm::vec3 Camera::getDir()
{
	return this-> dir;
}

glm::vec3 Camera::getPos()
{
	return pos;
}

int Camera::getWidth()
{
	return width;
}

int Camera::getHeight()
{
	return height;
}

float Camera::getNear()
{
	return NEAR_POINT;
}

float Camera::getFar()
{
	return FAR_POINT;
}

void Camera::setDir(const glm::vec3& dir)
{
	this->dir = dir;
	updateMatrices();
}

void Camera::setPos(const glm::vec3& pos)
{
	this->pos = pos;
	updateMatrices();
}

void Camera::setDim(int w, int h)
{
	setWidth(w);
	setHeight(h);
}

void Camera::setWidth(int w)
{
	width = w;
	updateMatrices();
}

void Camera::setHeight(int h)
{
	height = h;
	updateMatrices();
}

