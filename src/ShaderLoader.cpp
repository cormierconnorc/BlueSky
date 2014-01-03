/*
 * ShaderLoader.cpp
 *
 *  Created on: May 25, 2013
 *      Author: Connor Cormier
 *      Based on code from OPENGL-TUTORIAL.ORG
 */

#include "ShaderLoader.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

GLuint loadShaders(const string& vertexFilePath, const string& fragmentFilePath)
{
	//Create shaders
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	//Read vertex shader file
	string vertexShaderCode = readFile(vertexFilePath);

	//read fragment shader
	string fragmentShaderCode = readFile(fragmentFilePath);

	//Result of whole process
	GLint result = GL_FALSE;
	int infoLogLength;

	//Compile vertex shader
	compileAndCheckShader(vertexShaderId, vertexFilePath, vertexShaderCode, result, infoLogLength);

	//Compile fragment shader
	compileAndCheckShader(fragmentShaderId, fragmentFilePath, fragmentShaderCode, result, infoLogLength);

	//Link the program
	cout << "Linking" << endl;
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	//Check the program
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> programErrorMessage(max(infoLogLength, 1));
	//Note: reference to first element of vector passed as if array, serves as constant pointer to first element
	glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMessage[0]);

	for (vector<char>::size_type i = 0; i < programErrorMessage.size(); i++)
		cout << programErrorMessage[i];
	cout << endl;


	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}

void compileAndCheckShader(const GLuint& shaderId, const string& shaderPath, const string& shaderCode, GLint& result, int& infoLogLength)
{
	//Compile vertex shader
	cout << "Compiling Shader: " << shaderPath << endl;

	const char * codePointer = shaderCode.c_str();
	glShaderSource(shaderId, 1, &codePointer, NULL); //NULL in all caps, damn c++
	glCompileShader(shaderId);

	//Check Vertex Shader
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> vectorShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(shaderId, infoLogLength, NULL, &vectorShaderErrorMessage[0]);

	for (vector<char>::size_type i = 0; i < vectorShaderErrorMessage.size(); i++)
		cout << vectorShaderErrorMessage[i];
	if (vectorShaderErrorMessage.size() > 0)
		cout << endl;
}

string readFile(const string& file)
{
	ifstream in(file.c_str());
	string text = "";

	if (in.is_open())
	{
		string line;

		while (getline(in, line))
			text += line + "\n";

		in.close();
	}

	return text;
}


