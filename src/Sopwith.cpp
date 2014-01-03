/*
 * Sopwith.cpp
 *
 *  Created on: Jul 16, 2013
 *      Author: Connor Cormier
 */

#include "Sopwith.h"
#include "BlueSky.h"
#include "Bullet.h"
#include <iostream>
#include <glm/gtx/vector_angle.hpp>

using namespace std;

constexpr float Sopwith::SCALE, Sopwith::SHOOT_DISTANCE, Sopwith::COLLISION_DISTANCE;
constexpr double Sopwith::COOLDOWN;

Sopwith::Sopwith() : Entity("SopwithCamel.obj", "Sopwith2.tga", SCALE, COLLISION_DISTANCE) {}
Sopwith::Sopwith(const glm::quat& orientation) : Entity("SopwithCamel.obj", "Sopwith2.tga", SCALE, COLLISION_DISTANCE, orientation) {}
Sopwith::Sopwith(const glm::vec3& pos) : Entity("SopwithCamel.obj", "Sopwith2.tga", SCALE, COLLISION_DISTANCE, pos) {}
Sopwith::Sopwith(const glm::vec3& pos, const glm::quat& orientation) : Entity("SopwithCamel.obj", "Sopwith2.tga", SCALE, COLLISION_DISTANCE, pos, orientation) {}
Sopwith::Sopwith(const glm::vec3& pos, const glm::vec3& rot) : Entity("SopwithCamel.obj", "Sopwith2.tga", SCALE, COLLISION_DISTANCE, pos, rot) {}

bool Sopwith::update()
{
	//Entity's update method calls collision check method and handles deletion flags
	if (Entity::update())
		return true;

	Camera* cam = getCam();

	bool facing = Entity::rotateTowards(cam->getPos(), rotSpeed);
	Entity::moveForward(tranSpeed * dT);

	if (facing && getDistance(cam->getPos()) < SHOOT_DISTANCE)
		fireBullet();

	return false;
}

void Sopwith::fireBullet()
{
	if (glfwGetTime() - lastShot >= COOLDOWN)
		lastShot = glfwGetTime();
	else
		return;

	//For use in centering bullets on guns
	float forFactor = 1.5f, upFactor = .42f, sideFactor = .075f;

	glm::vec3 bulletPosition = getPos();
	bulletPosition = bulletPosition + (getForward() * forFactor) + (getUp() * upFactor);

	//Offset to one of the guns or the other
	if (bulletsFired % 2 == 0)
	{
		bulletPosition = bulletPosition + (getRight() * sideFactor);
		bulletsFired = 0;
	}
	else
		bulletPosition = bulletPosition - (getRight() * sideFactor);

	Entity* bul = new Bullet(bulletPosition, getQuaternion());
	addEntity(bul);

	bulletsFired++;
}

float Sopwith::getRotSpeed()
{
	return rotSpeed;
}

float Sopwith::getTranSpeed()
{
	return tranSpeed;
}
