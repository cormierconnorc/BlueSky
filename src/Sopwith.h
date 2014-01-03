/*
 * Sopwith.h
 *
 *  Created on: Jul 16, 2013
 *      Author: Connor Cormier
 */

#ifndef SOPWITH_H_
#define SOPWITH_H_

#include "Entity.h"

class Sopwith: public Entity
{
private:
	static constexpr float SCALE = .01f, SHOOT_DISTANCE = 40.0f, COLLISION_DISTANCE = 1.0f;
	//Cooldown in seconds
	static constexpr double COOLDOWN = .1;
	double lastShot = 0.0;
	//Old rot: .25f
	float rotSpeed = .25f, tranSpeed = 10.0f;
	short bulletsFired = 0;
public:
	Sopwith();
	Sopwith(const glm::quat&);
	Sopwith(const glm::vec3&);
	Sopwith(const glm::vec3&, const glm::quat&);
	Sopwith(const glm::vec3&, const glm::vec3&);
	virtual bool update();
	void fireBullet();
	float getRotSpeed(), getTranSpeed();
};


#endif /* SOPWITH_H_ */
