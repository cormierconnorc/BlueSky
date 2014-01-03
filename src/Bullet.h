/*
 * Bullet.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Connor Cormier
 */

#ifndef BULLET_H_
#define BULLET_H_

#include "Entity.h"

class Bullet : public Entity
{
private:
	static constexpr float SCALE = .01f, SPEED = 100.0f, COLLISION_DISTANCE = .1f;
public:
	Bullet(const glm::vec3&, const glm::quat&);
	virtual bool update();
};


#endif /* BULLET_H_ */
