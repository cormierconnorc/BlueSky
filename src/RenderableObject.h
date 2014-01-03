/*
 * RenderableObject.h
 *
 *  Created on: Jul 15, 2013
 *      Author: Connor Cormier
 */

#ifndef RENDERABLEOBJECT_H_
#define RENDERABLEOBJECT_H_

#include <vector>
#include <glm/glm.hpp>


struct RenderableObject
{
	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> uvs;
	GLuint vertexBuffer, uvBuffer, texId, programId;
};


#endif /* RENDERABLEOBJECT_H_ */
