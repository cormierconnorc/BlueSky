/*
 * ObjLoader.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: Connor Cormier
 *      Based on code from OPENGL-TUTORIAL.ORG
 */

#include "ObjLoader.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

bool loadObj(const string& filePath, vector<glm::vec3>& vertices, vector<glm::vec2>& uvs, vector<glm::vec3>& normals)
{
	FILE* file = fopen(filePath.c_str(), "r");

	if (file == NULL)
	{
		cout << "Cannot open file" << endl;
		return false;
	}

	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<glm::vec3> tempVertices;
	vector<glm::vec2> tempUVs;
	vector<glm::vec3> tempNormals;

	while (true)
	{
		char lineHeader[128];

		//Read first word of line
		int res = fscanf(file, "%s", lineHeader);

		if (res == EOF)	//EOF is End Of File
			break;

		//If line starts with v/is vertex
		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			//Read 3 floats on line into vector components
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
		}
		//If it's a texture coord
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			//Read next 2 floats into uv components
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			//If using DDS textures, uncomment
			//uv.y = -uv.y;
			tempUVs.push_back(uv);
		}
		//If a vertex normal
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tempNormals.push_back(normal);
		}
		//f determines which vertex/texture coordinate/normal to use for each face
		else if (strcmp(lineHeader, "f") == 0)
		{
			//3 arrays to hold the data on this line
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			if (matches != 9)
			{
				cout << "File incompatible with simple parser" << endl;
				return false;
			}

			//Now add each of the indices to the vector
			for (int i = 0; i < 3; i++)
			{
				vertexIndices.push_back(vertexIndex[i]);
				uvIndices.push_back(uvIndex[i]);
				normalIndices.push_back(normalIndex[i]);
			}
		}
		//Comment, skip
		else
		{
			char commentBuffer[1000];
			fgets(commentBuffer, 1000, file);
		}
	}

	//Indexing to get data in a format OpenGL can use
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		//Get number vertex of i(th) vertex to draw
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		//Get vectors corresponding to vertices to draw at position i
		//Offset of 1 is needed to change obj indices (which start at 1) to c++ indices
		glm::vec3 vertex = tempVertices[vertexIndex - 1];
		glm::vec2 uv = tempUVs[uvIndex - 1];
		glm::vec3 normal = tempNormals[normalIndex - 1];

		//Put in order in output vectors
		vertices.push_back(vertex);
		uvs.push_back(uv);
		normals.push_back(normal);
	}

	return true;
}

