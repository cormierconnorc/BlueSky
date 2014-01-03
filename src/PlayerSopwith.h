/*
 * PlayerSopwith.h
 *
 *  Created on: Jul 21, 2013
 *      Author: Connor Cormier
 */

#ifndef PLAYERSOPWITH_H_
#define PLAYERSOPWITH_H_

#include "Sopwith.h"
#include "SlaveCamera.h"

class PlayerSopwith : public Sopwith
{
private:
	static constexpr float UP_FACTOR = .41f;
	static constexpr float FOR_FACTOR = .77f;
	const float MAX_VELOCITY = getTranSpeed() * 1.1f, MIN_VELOCITY = MAX_VELOCITY / 4, MAX_ROT_SPEED = getRotSpeed() * 1.0f, ACCEL = .01f,
			X_ROT_FACTOR = 7.5f, Y_ROT_FACTOR = .5f, Z_ROT_FACTOR = 7.5f;
	float velocity = MAX_VELOCITY;
	int hitPoints = 5;
	const float MAX_X_ROT = 70.0f * 3.1415926f / 180.0f, MAX_Y_ROT = 150.0f * 3.1415926f / 180.0f;
	float camRotX = 0.0f, camRotY = 0.0f;
	Camera* cam;
public:
	PlayerSopwith(Camera*);
	PlayerSopwith(Camera*, const glm::quat&);
	PlayerSopwith(Camera*, const glm::vec3&);
	PlayerSopwith(Camera*, const glm::vec3&, const glm::quat&);
	PlayerSopwith(Camera*, const glm::vec3&, const glm::vec3&);
	virtual bool update();
	void acceptInput();
};


#endif /* PLAYERSOPWITH_H_ */
