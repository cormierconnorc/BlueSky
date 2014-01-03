/*
 * Terrain.cpp
 *
 *  Created on: Jul 17, 2013
 *      Author: Connor Cormier
 */

#include <ctime>
#include <iostream>
#include <glm/gtx/transform.hpp>

#include "Terrain.h"
#include "Perlin.h"
#include "ShaderLoader.h"

using namespace std;

constexpr float TRANS_DOWN = -50.0f;

//Load shaders associated with terrain
Terrain::Terrain() : PROGRAM_ID(loadShaders("colorVertexShader.glsl", "colorFragmentShader.glsl"))
{

}

void Terrain::genTerrain(const glm::vec3& center, float side)
{
	this->center = center;
	this->side = side;

	//NOT THE PROBLEM
	//Set up initial vector of values
	//Working values: 20.0f for render 3rd parameter, 1.0f for scale, 5.0f for ampScale
	vector<vector<float> > heightMap = render(side, side, ZOOM, .5f);
	//Vector to hold vertex values
	//Length of triangle legs is 1 / scale
	float scale = SCALE, ampScale = AMP;

	for (vector<vector<float> >::size_type z = 0; z < heightMap.size() - 1; z++)
		for (vector<float>::size_type x = 0; x < heightMap[z].size() - 1; x++)
		{
			//Draw two triangles off of the heightmap and r c values
			//Triangle 1 vertex 1 x y z
			addVertex(glm::vec3(x / scale, heightMap[z][x] * ampScale, z / scale));
			addVertex(glm::vec3((x + 1) / scale, heightMap[z + 1][x + 1] * ampScale, (z + 1) / scale));
			addVertex(glm::vec3(x / scale, heightMap[z + 1][x] * ampScale, (z + 1) / scale));
			//Now other triangle to make square
			addVertex(glm::vec3(x / scale, heightMap[z][x] * ampScale, z / scale));
			addVertex(glm::vec3((x + 1) / scale, heightMap[z][x + 1] * ampScale, z / scale));
			addVertex(glm::vec3((x + 1) / scale, heightMap[z + 1][x + 1] * ampScale, (z + 1) / scale));
		}

	//Vertices is now a proper representation of the heightmap. Move on to generating terrainColors by rands between min green and max green values
	//Prepare to get random number
	srand(time(NULL));

	for (vector<glm::vec3>::size_type i = 0; i < terrainVertices.size(); i++)
	{
		//g range is [.501, .6]
		float g;
		g = (rand() % /*100*/50 + 501) / 1000.0f;
		terrainColors.push_back(glm::vec3(0.0f, g, 0.0f));
	}

	//Update buffers
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, terrainVertices.size() * sizeof(glm::vec3), &terrainVertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, terrainColors.size() * sizeof(glm::vec3), &terrainColors[0], GL_STATIC_DRAW);
	//The index buffer
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(vector<glm::vec3>::size_type), &indices[0], GL_STATIC_DRAW);

}

//Add to vector and map
void Terrain::addVertex(const glm::vec3& vertex)
{
	//Now uses VBO indexing. WORKS PERFECTLY!

	std::pair<std::pair<float, float>, float > v = make_pair(make_pair(vertex.x, vertex.y), vertex.z);

	if (!iMap.count(v))
	{
		//vMap handles heights at position
		vMap[make_pair(vertex.x, vertex.z)] = vertex.y;
		//Add current vertex index to buffer map
		iMap[v] = terrainVertices.size();
		//Index buffer handles occurrences of vertices
		indices.push_back(terrainVertices.size());
		//Terrain vertices holds actual vertex
		terrainVertices.push_back(vertex);
	}
	else
		indices.push_back(iMap[v]);
}

void Terrain::update(Camera* cam)
{
	if (getDistanceToSide(cam->getPos()) < side / 10)
		setCenter(cam->getPos());
	drawTerrain(cam);
}

void Terrain::drawTerrain(Camera* cam)
{
	//Tell program to use shaders
	glUseProgram(PROGRAM_ID);

	//Start by updating matrices (mvp)
	glm::mat4 mvp = cam->getProjectionMatrix() * cam->getViewMatrix() * getModelMatrix();

	//Give matrix to glsl
	GLuint matrixId = glGetUniformLocation(PROGRAM_ID, "mvp");
	//Send transformation to currently bound shader
	glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

	glEnableVertexAttribArray(0);
	//Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//Set attributes (confusing)
	glVertexAttribPointer(
			0,			//Matches attrib array number
			3, 			//Size
			GL_FLOAT, 	//Type
			GL_FALSE, 	//Normalized
			0, 			//Stride
			(void*)0	//Array buffer offset
			);

	//On to binding colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);

	//Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	//Now draw the terrain
	//glDrawArrays(GL_TRIANGLES, 0, terrainVertices.size());
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

	//Disable the attrib arrays
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Terrain::setCenter(const glm::vec3& center)
{
	this->center = center;
}

float Terrain::getDistanceToSide(const glm::vec3& pos)
{
	//Positive and negative x (side / SCALE) distance
	float pX = abs((center.x + (side / SCALE) / 2) - pos.x);
	float nX = abs((center.x - (side / SCALE) / 2) - pos.x);
	//Positive and negative z (side / SCALE) difference
	float pZ = abs((center.z + (side / SCALE) / 2) - pos.z);
	float nZ = abs((center.z - (side / SCALE) / 2) - pos.z);

	//Return minimum value
	return min(min(pX, nX), min(pZ, nZ));
}

float Terrain::getDistanceToGround(const glm::vec3& pos)
{
	float tH = getTerrainHeight(pos);
	return pos.y - tH;
}

//Get height of terrain at position. Needs to be optimized
float Terrain::getTerrainHeight(const glm::vec3& pos)
{
	//Align position to terrain array
	float x = (int)(pos.x - center.x + (side / SCALE) / 2);
	float z = (int)(pos.z - center.z + (side / SCALE) / 2);

	pair<float, float> p = make_pair(x, z);

	if (vMap.count(p))
		return vMap[p] + TRANS_DOWN;
	return TRANS_DOWN;
}

glm::mat4 Terrain::getModelMatrix()
{
	glm::mat4 mat = glm::translate(center.x - (side / SCALE) / 2, TRANS_DOWN, center.z - (side / SCALE) / 2) * glm::mat4(1.0f);
	return mat;
}



