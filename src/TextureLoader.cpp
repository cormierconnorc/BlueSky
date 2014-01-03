/*
 * TextureLoader.cpp
 *
 *  Created on: May 25, 2013
 *      Author: Connor Cormier
 *      Based on code from OPENGL-TUTORIAL.ORG
 */

#include "TextureLoader.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

GLuint loadTGATexture(const string& texPath)
{
	//Generate a textureId
	GLuint textureId;
	glGenTextures(1, &textureId);

	//Bind the newly created texture; all subsequent methods will operate on it.
	glBindTexture(GL_TEXTURE_2D, textureId);

	//Read and load file using glfw
	glfwLoadTexture2D(texPath.c_str(), 0);

	//Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureId;
}

GLuint loadBMPTexture(const string& texPath)
{
	//Data to be read from BMP header
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;		//width * height * 3

	//Open the file
	FILE * file = fopen(texPath.c_str(), "rb");
	if (!file)
	{
		cout << "Image could not be opened" << endl;
		return 0;
	}

	//If not 54 byte header, wrong type
	if (fread(header, 1, 54, file) != 54)
	{
		cout << "Not a correct BMP file" << endl;
		return 0;
	}

	//Check if correct BMP
	if (header[0] != 'B' || header[1] != 'M')
	{
		cout << "Incorrect BMP file" << endl;
		return 0;
	}

	//Now data from file
	dataPos = *(int*)&header[0x0A];
	imageSize = *(int*)&header[0x22];
	width = *(int*)&header[0x12];
	height = *(int*)&header[0x16];

	//If data is missing, make it up
	if (imageSize == 0)
		imageSize = width * height * 3;		//1 byte for R, G, and B of each pixel

	if (dataPos == 0)
		dataPos = 54;		//header size in bytes

	//The data array
	char data[imageSize];

	//Read data from file into buffer
	fread(data, 1, imageSize, file);

	//Now close file since data is in memory
	fclose(file);

	//Create the texture
	GLuint textureId;
	glGenTextures(1, &textureId);

	//Bind the texture so all subsequent operations work on it
	glBindTexture(GL_TEXTURE_2D, textureId);

	//Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	//Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureId;
}

//Define ASCII values of strings in DDS header
#define FOURCC_DXT1 0x31545844	//DXT1 ASCII value
#define FOURCC_DXT3 0x33545844	//DXT3 ASCII value
#define FOURCC_DXT5 0x35545844	//DXT5 ASCII value

GLuint loadDDSTexture(const string& texPath)
{
	unsigned char header[124];

	FILE *fp = fopen(texPath.c_str(), "rb");

	if (!fp)
		return 0;

	char fileCode[4];
	fread(fileCode, 1, 4, fp);

	//Compare strings. 0 means equal, >0 means str1 is greater, <0 means str2 is greater. Like compare method
	if (strncmp(fileCode, "DDS ", 4) != 0)
	{
		fclose(fp);
		return 0;
	}


	//Read the surface description
	//Pointer to start of array, size of each element to be read, number of elements, file input stream
	fread(&header, 124, 1, fp);

	//Get image data from array
	unsigned int height = *(unsigned int*)&header[8];
	unsigned int width = *(unsigned int*)&header[12];
	unsigned int linearSize = *(unsigned int*)&header[16];
	unsigned int mipMapCount = *(unsigned int*)&header[24];
	unsigned int fourCC = *(unsigned int*)&header[80];

	//Read mipmap data
	unsigned int bufSize;
	bufSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	unsigned char buffer[bufSize];

	//Read data into buffer
	fread(buffer, 1, bufSize, fp);

	unsigned int format;

	//Now select proper DXT format (1, 3, or 5) based on data in header
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		return 0;
	}

	//Now create the texture
	GLuint textureId;
	glGenTextures(1, &textureId);

	//Bind texture
	glBindTexture(GL_TEXTURE_2D, textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Now fill each mipmap level of the texture
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	//Load the mipmaps
	for (unsigned int level = 0; level < mipMapCount && (width || height); level++)
	{
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}

	return textureId;
}

