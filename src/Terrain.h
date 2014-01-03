/*
 * Terrain.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Connor Cormier
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <GL/glew.h>
#include <GL/glfw.h>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "Camera.h"

class Terrain
{
private:
	//Working values: SCALE = 1.0f, ZOOM = 20.0f, AMP = 5.0f
	static constexpr float SCALE = 1.0f, ZOOM = 20.0f, AMP = 5.0f;
	static constexpr float TRANS_DOWN = -50.0f;
	const GLuint PROGRAM_ID;
	glm::vec3 center;
	float side;
	std::vector<glm::vec3> terrainVertices, terrainColors;
	std::vector<glm::vec3::size_type> indices;
	std::map<std::pair<float, float>, float> vMap;
	std::map<std::pair<std::pair<float, float>, float >, std::vector<glm::vec3>::size_type> iMap;
	GLuint vertexBuffer, colorBuffer, indexBuffer;
	void addVertex(const glm::vec3&);
public:
	Terrain();
	void genTerrain(const glm::vec3&, float);
	void update(Camera*);
	void drawTerrain(Camera*);
	void setCenter(const glm::vec3&);
	float getDistanceToSide(const glm::vec3&);
	float getDistanceToGround(const glm::vec3&);
	float getTerrainHeight(const glm::vec3&);
	glm::mat4 getModelMatrix();
};


#endif /* TERRAIN_H_ */
