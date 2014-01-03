/*
 * Bullet.cpp
 *
 *  Created on: Jul 17, 2013
 *      Author: Connor Cormier
 */

#include "Bullet.h"
#include "BlueSky.h"

using namespace std;

constexpr float Bullet::SCALE, Bullet::SPEED, Bullet::COLLISION_DISTANCE;

Bullet::Bullet(const glm::vec3& pos, const glm::quat& orientation) : Entity("bullet.obj", "bullet.tga", SCALE, COLLISION_DISTANCE, pos, orientation) {}

bool Bullet::update()
{
	if (Entity::update())
		return true;

	moveForward(SPEED * dT);

	//Remove from screen if out of view
	if (getDistance(getCam()->getPos()) > getCam()->getFar())
		return true;
	return false;
}

