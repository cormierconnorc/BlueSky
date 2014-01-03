/*
 * Camera.h
 *
 *  Created on: May 25, 2013
 *      Author: Connor Cormier
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera
{
private:
	static constexpr float ROT_SPEED = 1.0f, TRAN_SPEED = 5.0f;
	static constexpr float NEAR_POINT = 0.01f, FAR_POINT = 500.0f;
	glm::quat orientation;
	double time, dT;
	int width, height;
	float fov;
	glm::mat4 projectionMatrix, viewMatrix;
	glm::vec3 dir, up, right, pos;
public:
	Camera(float, int, int);
	Camera(float, int, int, glm::vec3, glm::vec3);
	virtual void update();
	void rotate();
	void translate();
	void updateTime();
	void updateMatrices();
	void resetOrientation();
	void rotateAbout(const glm::quat&);
	glm::vec3 rotateVector(const glm::mat4&, const glm::vec3&);
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	void setDir(const glm::vec3&), setPos(const glm::vec3&);
	void setOrientation(const glm::quat&);
	glm::vec3 getDir(), getPos();
	void setDim(int, int), setWidth(int), setHeight(int);
	int getWidth(), getHeight();
	float getNear(), getFar();
};


#endif /* CAMERA_H_ */
