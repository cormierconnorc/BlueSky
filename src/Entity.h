/*
 * Entity.h
 *
 *  Created on: Jul 15, 2013
 *      Author: Connor Cormier
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <map>
#include <string>
#include "RenderableObject.h"
#include "Camera.h"

class Entity
{
private:
	const float COLLISION_DISTANCE;
	static std::map<std::string, RenderableObject> loaded;
	RenderableObject obj;
	//Note: initialized in written order
	double time;
	glm::vec3 pos;
	float scale;
	void load(const std::string&, const std::string&);
	bool deletionFlag = false;
protected:
	glm::quat orientation;
	float dT;
public:
	Entity(const std::string&, const std::string&, float, float);
	Entity(const std::string&, const std::string&, float, float, const glm::quat&);
	Entity(const std::string&, const std::string&, float, float, const glm::vec3&);
	Entity(const std::string&, const std::string&, float, float, const glm::vec3&, const glm::quat&);
	Entity(const std::string&, const std::string&, float, float, const glm::vec3&, const glm::vec3&);
	void render(Camera*);
	void flagDelete();
	virtual bool rotateTowards(const glm::vec3&, float), rotateTowards(float, float, float), rotateTowards(const glm::quat& des, float);
	void rotateRelative(const glm::vec3&);
	void rotateRelativeX(float), rotateRelativeY(float), rotateRelativeZ(float);
	void rotateAbsolute(const glm::vec3&);
	void rotateX(float), rotateY(float), rotateZ(float);
	void moveRel(const glm::vec3&), moveForward(float), moveUp(float), moveRight(float);
	void moveAbs(const glm::vec3&), moveX(float), moveY(float), moveZ(float);
	void setOrientation(const glm::vec3&), setOrientation(const glm::quat&);
	void setVals(const glm::vec3&, const glm::vec3&), setPos(const glm::vec3&);
	void setX(float), setY(float), setZ(float);
	void setScale(float);
	glm::quat getQuaternion();
	glm::mat4 getRotationMatrix();
	glm::mat4 getModelMatrix();
	glm::vec3 getPos();
	float getX(), getY(), getZ();
	float getScale();
	glm::vec3 getRight(), getUp(), getForward(), getRight(const glm::mat4&), getUp(const glm::mat4&), getForward(const glm::mat4&),
			rotateVec(const glm::mat4&, const glm::vec3&), getLookVector(const glm::vec3&);
	float getDistance(const glm::vec3&);
	glm::vec3 getEuler(), getEulerRads();
	//No longer true virtual. Now handles collision checking and deletion flags
	virtual bool update();
	//Now true virtual
	virtual bool checkCollision();
	bool checkGroundCollision(), checkEntityCollision();
};


#endif /* ENTITY_H_ */
