/*
 * ObjLoader.h
 *
 *  Created on: Jun 6, 2013
 *      Author: Connor Cormier
 */

#ifndef OBJLOADER_H_
#define OBJLOADER_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>

bool loadObj(const std::string&, std::vector<glm::vec3>&, std::vector<glm::vec2>&, std::vector<glm::vec3>&);


#endif /* OBJLOADER_H_ */
