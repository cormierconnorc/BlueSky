/*
 * Entity.cpp
 *
 *  Created on: Jul 15, 2013
 *      Author: Connor Cormier
 */

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <list>
#include "Entity.h"
#include "ObjLoader.h"
#include "TextureLoader.h"
#include "BlueSky.h"
#include "ShaderLoader.h"

using namespace std;

//Static variables must be defined somewhere outside of the class if not constant
map<string, RenderableObject> Entity::loaded;

Entity::Entity(const std::string& objFile, const std::string& texFile, float scale, float cDistance) : COLLISION_DISTANCE(cDistance), time(glfwGetTime()), pos(0, 0, 0), scale(scale)
{
	load(objFile, texFile);
}

Entity::Entity(const std::string& objFile, const std::string& texFile, float scale, float cDistance, const glm::quat& orientation) : COLLISION_DISTANCE(cDistance), time(glfwGetTime()), pos(0, 0, 0), scale(scale), orientation(orientation)
{
	load(objFile, texFile);
}

Entity::Entity(const std::string& objFile, const std::string& texFile, float scale, float cDistance, const glm::vec3& pos, const glm::quat& orientation) : COLLISION_DISTANCE(cDistance), time(glfwGetTime()), pos(pos), scale(scale), orientation(orientation)
{
	load(objFile, texFile);
}

Entity::Entity(const std::string& objFile, const std::string& texFile, float scale, float cDistance, const glm::vec3& pos) : COLLISION_DISTANCE(cDistance), time(glfwGetTime()), pos(pos), scale(scale)
{
	load(objFile, texFile);
}

Entity::Entity(const std::string& objFile, const std::string& texFile, float scale, float cDistance, const glm::vec3& pos, const glm::vec3& rot) : COLLISION_DISTANCE(cDistance), time(glfwGetTime()), pos(pos), scale(scale), orientation(rot)
{
	load(objFile, texFile);
}

//Load the object file into a renderable object. Needed to draw later on
void Entity::load(const string& objFile, const string& texFile)
{
	//Attempt to get obj file from map and return first
	string objRef = objFile + " " + texFile;

	if (loaded.count(objRef))
	{
		obj = loaded[objRef];
		return;
	}

	if (loaded.size() == 0)
		obj.programId = loadShaders("vertexShader.glsl", "fragmentShader.glsl");
	else
		//Same programId, so just use an old one
		obj.programId = loaded.begin()->second.programId;

	bool loaded = loadObj(objFile, obj.vertices, obj.uvs, obj.normals);

	if (!loaded)
		throw exception();

	//Generate buffer
	glGenBuffers(1, &obj.vertexBuffer);
	//Bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, obj.vertexBuffer);
	//Give vertices to opengl
	glBufferData(GL_ARRAY_BUFFER, obj.vertices.size() * sizeof(glm::vec3), &obj.vertices[0], GL_STATIC_DRAW);
	//Same for the uv map
	glGenBuffers(1, &obj.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, obj.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, obj.uvs.size() * sizeof(glm::vec2), &obj.uvs[0], GL_STATIC_DRAW);

	//Now get texture ID
	obj.texId =	loadTGATexture(texFile);

	//Now add to map for future reference
	this->loaded[objRef] = obj;
}

//Draw the Sopwith model
void Entity::render(Camera* cam)
{
	//Tell program to use shaders
	glUseProgram(obj.programId);

	//Start by updating matrices (mvp)
	glm::mat4 mvp = cam->getProjectionMatrix() * cam->getViewMatrix() * getModelMatrix();
	//Give matrix to glsl
	GLuint matrixId = glGetUniformLocation(obj.programId, "mvp");
	//Send transformation to currently bound shader
	glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

	//Now bind the proper buffers, texture first
	glBindTexture(GL_TEXTURE_2D, obj.texId);

	glEnableVertexAttribArray(0);
	//Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, obj.vertexBuffer);
	//Set attributes (confusing)
	glVertexAttribPointer(
			0,			//Matches attrib array number
			3, 			//Size
			GL_FLOAT, 	//Type
			GL_FALSE, 	//Normalized
			0, 			//Stride
			(void*)0	//Array buffer offset
			);

	//On to binding UVs

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, obj.uvBuffer);
	glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);

	//Now draw the camel
	glDrawArrays(GL_TRIANGLES, 0, obj.vertices.size());

	//Disable the attrib arrays
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

//Flag entity for deletion on next update call
void Entity::flagDelete()
{
	deletionFlag = true;
}

//Entity update method handles three things: call to collision checking method, deletion flags, and time updates
bool Entity::update()
{
	dT = (float)(glfwGetTime() - time);
	time = glfwGetTime();

	if (deletionFlag)
		return true;
	return checkCollision();
}

bool Entity::checkCollision()
{
	if (checkGroundCollision())
		return true;

	return checkEntityCollision();
}

bool Entity::checkEntityCollision()
{
	list<Entity*> entities = getEntityList();
	for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
		//Get distance
		if (this != (*it) && getDistance((*it)->getPos()) < COLLISION_DISTANCE)
		{
			(*it)->flagDelete();
			return true;
		}
	return false;
}

bool Entity::checkGroundCollision()
{
	//Check collision with ground first
	Terrain* t = getTerrain();
	if (t->getDistanceToGround(getPos()) < COLLISION_DISTANCE)
		return true;
	return false;
}

//Rotate towards a point
bool Entity::rotateTowards(const glm::vec3& point, float rotSpeed)
{
	glm::vec3 d = getLookVector(point);

	//Calculate rotY
	float rotY = atan2(d.x, d.z);
	//Calculate rotX
	float rotX = -atan2(d.y, sqrt(d.x*d.x + d.z*d.z));

	return rotateTowards(rotX, rotY, rotSpeed);
}

//Rotate towards an angle
bool Entity::rotateTowards(float rotX, float rotY, float rotSpeed)
{
	glm::quat target = glm::quat(glm::vec3(rotX, rotY, 0.0f));
	return rotateTowards(target, rotSpeed);
}

//Rotate towards a quaternion. Returns whether turn has completed
bool Entity::rotateTowards(const glm::quat& q, float rotSpeed)
{
	//Adjust rotSpeed to time. Consistent regardless of framerate
	rotSpeed *= dT;

	float cos = glm::dot(orientation, q);

	//Already equal
	if (cos > .9999f)
	{
		orientation = q;
		return true;
	}

	//Quickest path around sphere
	if (cos < 0)
	{
		orientation = orientation * -1.0f;
		cos *= -1.0f;
	}

	float angle = acos(cos);
	//Close enough to reach in this move
	if (angle < rotSpeed)
	{
		orientation = q;
		return true;
	}

	float mixVal = rotSpeed / angle;
	angle = rotSpeed;

	//Perform interpolation
	orientation = glm::normalize((sin((1.0f - mixVal) * angle) * orientation + sin(mixVal * angle) * q) / sin(angle));

	return false;
}

void Entity::rotateRelative(const glm::vec3& rot)
{
	orientation = orientation * glm::quat(rot);
}

//Rotate about the x axis relative to the current orientation
void Entity::rotateRelativeX(float rX)
{
	orientation = orientation * glm::quat(glm::vec3(rX, 0.0f, 0.0f));
}

void Entity::rotateRelativeY(float rY)
{
	orientation = orientation * glm::quat(glm::vec3(0.0f, rY, 0.0f));
}

void Entity::rotateRelativeZ(float rZ)
{
	orientation = orientation * glm::quat(glm::vec3(0.0f, 0.0f, rZ));
}

void Entity::rotateAbsolute(const glm::vec3& rot)
{
	orientation = glm::quat(rot) * orientation;
}

void Entity::rotateX(float rX)
{
	orientation = glm::quat(glm::vec3(rX, 0.0f, 0.0f)) * orientation;
}

void Entity::rotateY(float rY)
{
	orientation = glm::quat(glm::vec3(0.0f, rY, 0.0f)) * orientation;
}

void Entity::rotateZ(float rZ)
{
	orientation = glm::quat(glm::vec3(0.0f, 0.0f, rZ)) * orientation;
}

void Entity::moveRel(const glm::vec3& change)
{
	moveForward(change.z);
	moveUp(change.y);
	moveRight(change.x);
}

void Entity::moveForward(float dz)
{
	glm::vec3 change = getForward() * dz;
	moveAbs(change);
}

void Entity::moveUp(float dy)
{
	glm::vec3 change = getUp() * dy;
	moveAbs(change);
}

void Entity::moveRight(float dx)
{
	glm::vec3 change = getRight() * dx;
	moveAbs(change);
}

void Entity::moveAbs(const glm::vec3& change)
{
	pos += change;
}

void Entity::moveX(float dx)
{
	pos.x += dx;
}

void Entity::moveY(float dy)
{
	pos.y += dy;
}

void Entity::moveZ(float dz)
{
	pos.z += dz;
}

glm::quat Entity::getQuaternion()
{
	return orientation;
}

glm::mat4 Entity::getRotationMatrix()
{
	return glm::mat4_cast(orientation);
}

glm::mat4 Entity::getModelMatrix()
{
	glm::mat4 rotationMatrix = getRotationMatrix();
	//Remember: scale, rotate, translate in that order
	glm::mat4 model = glm::translate(pos.x, pos.y, pos.z) * rotationMatrix * glm::scale(this->scale, this->scale, this->scale);

	return model;
}

glm::vec3 Entity::getPos() {return pos;}
float Entity::getX() {return pos.x;}
float Entity::getY() {return pos.y;}
float Entity::getZ() {return pos.z;}
float Entity::getScale() {return scale;}

//get right vector using current rotation matrix
glm::vec3 Entity::getRight() { return getRight(getRotationMatrix()); }
//get Up vector using current rotation matrix
glm::vec3 Entity::getUp() { return getUp(getRotationMatrix()); }
//get Forward vector using current rotation matrix
glm::vec3 Entity::getForward() { return getForward(getRotationMatrix()); }

//get right vector using specified rotation matrix
glm::vec3 Entity::getRight(const glm::mat4& rot)
{
	glm::vec3 right(1.0f, 0.0f, 0.0f);
	return rotateVec(rot, right);
}

//Get up vector using specified rotation matrix
glm::vec3 Entity::getUp(const glm::mat4& rot)
{
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	return rotateVec(rot, up);
}

//Get forward vector using specified rotation matrix
glm::vec3 Entity::getForward(const glm::mat4& rot)
{
	glm::vec3 forward(0.0f, 0.0f, 1.0f);
	return rotateVec(rot, forward);
}

glm::vec3 Entity::rotateVec(const glm::mat4& matrix, const glm::vec3& vec)
{
	glm::vec4 out = matrix * glm::vec4(vec, 0.0f);
	glm::vec3 ret(out.x, out.y, out.z);
	ret = glm::normalize(ret);
	return ret;
}

glm::vec3 Entity::getLookVector(const glm::vec3& point)
{
	return 	glm::vec3(point.x - pos.x, point.y - pos.y, point.z - pos.z);
}

float Entity::getDistance(const glm::vec3& point)
{
	glm::vec3 lVec = getLookVector(point);
	return sqrt(lVec.x * lVec.x + lVec.y * lVec.y + lVec.z * lVec.z);
}

glm::vec3 Entity::getEuler()
{
	return glm::eulerAngles(orientation);
}

glm::vec3 Entity::getEulerRads()
{
	return (glm::eulerAngles(orientation) * (3.1415926f / 180));
}

void Entity::setOrientation(const glm::vec3& rot)
{
	orientation = glm::quat(rot);
}
void Entity::setOrientation(const glm::quat& orientation)
{
	this->orientation = orientation;
}

void Entity::setVals(const glm::vec3& pos, const glm::vec3& rot)
{
	setPos(pos);
	setOrientation(rot);
}
void Entity::setPos(const glm::vec3& pos)
{
	this->pos = pos;
}
void Entity::setX(float x) {pos.x = x;}
void Entity::setY(float y) {pos.y = y;}
void Entity::setZ(float z) {pos.z = z;}
void Entity::setScale(float scale) {this->scale = scale;}



