/*
 * TextureLoader.h
 *
 *  Created on: May 25, 2013
 *      Author: Connor Cormier
 */

#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_

#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>

GLuint loadTGATexture(const std::string&),
		loadBMPTexture(const std::string&),
		loadDDSTexture(const std::string&);

#endif /* TEXTURELOADER_H_ */
