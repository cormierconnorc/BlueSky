/*
 * BlueSky.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Connor Cormier
 */

#ifndef BLUESKY_H_
#define BLUESKY_H_

#include "Camera.h"
#include "Terrain.h"
#include <list>

Camera* getCam();
Terrain* getTerrain();
void addEntity(Entity*);
std::list<Entity*> getEntityList();

#endif /* BLUESKY_H_ */
