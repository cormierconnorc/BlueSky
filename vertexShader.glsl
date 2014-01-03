#version 330 core

//Inputs and locations
layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 1) in vec3 vertexColor;
layout(location = 1) in vec2 vertexUV;

//Set output to fragment shader
out vec2 uv;
uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(vertexPosition_modelspace, 1);
	//Set output to fragment shader
	uv = vertexUV;
}