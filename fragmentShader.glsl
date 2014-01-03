#version 330 core

//Get input color
in vec2 uv;
//Sets color of each fragment to red. Since we're using 4x AA, that's 4 fragments.
out vec3 color;

uniform sampler2D textureSampler;

void main()
{
	//Interpolated between surrounding vertices
	color = texture(textureSampler, uv).rgb;
	//color = vec3(1, 0, 0);
}