/*
 * ShaderLoader.h
 *
 *  Created on: May 25, 2013
 *      Author: Connor Cormier
 */

#ifndef SHADERLOADER_H_
#define SHADERLOADER_H_

#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>

GLuint loadShaders(const std::string&, const std::string&);
std::string readFile(const std::string&);
void compileAndCheckShader(const GLuint&, const std::string&, const std::string&, GLint&, int&);

#endif /* SHADERLOADER_H_ */
