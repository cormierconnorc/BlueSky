/*
 * Perlin.cpp
 *
 *  Created on: May 11, 2013
 *      Author: Connor Cormier
 */

#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include "Perlin.h"

using namespace std;

/*
 * What follows is based on an online tutorial for
 * Perlin noise. I understand very little of it.
 * Hooray!
 */

//Generate a random number between -1 and 1.
float findNoise2D(float x, float y)
{
	int n = (int)x + (int)y * 57;
	n = (n<<13)^n;
	int nn = (n * (n * n * 60493 + 19990303) + 1376312589)&0x7fffffff;
	return 1 - ((float)nn/1073741824);
}

//Cosine interpolation to create smooth curve.
float interpolate(float a, float b, float x)
{
	float ft = x * 3.1415926;
	float f = (1 - cos(ft)) * .5;
	return a * (1 - f) + b * f;
}

//Get the actual value for a pixel based on the surrounding values.
float getNoise(float x, float y)
{
	float floorX = (int)x;
	float floorY = (int)y;
	float s, t, u, v;

	//Noise values of all surrounding pixels
	s = findNoise2D(floorX, floorY);
	t = findNoise2D(floorX + 1, floorY);
	u = findNoise2D(floorX, floorY + 1);
	v = findNoise2D(floorX + 1, floorY + 1);

	float row1Val = interpolate(s, t, x - floorX);
	float row2Val = interpolate(u, v, x - floorX);

	float pixelVal = interpolate(row1Val, row2Val, y - floorY);

	return pixelVal;
}

/*
 * zoom determines how zoomed in or out image is. 75 is default.
 * p is persistence, the roughness of the picture. Default should be about 1/2
 */
vector<vector<float> > render(int width, int height, float zoom, float p)
{
	vector<vector<float> > values;

	int octaves = 2;

	for (int y = 0; y < height; y++)
	{
		values.push_back(vector<float>());

		for (int x = 0; x < width; x++)
		{
			float noise = 0;

			for (int i = 0; i < octaves - 1; i++)
			{
				float frequency = pow(2, i);	//Increasing frequency with each octave
				float amplitude = pow(p, i);	//Decreasing amplitude with each octave
				//Now use noise function to calculate zoom, frequency, and amplitude
				//Returns a decimal value between the pixels
				noise += getNoise(x * frequency / zoom, y / (zoom * frequency) * amplitude);
			}


			values[y].push_back(noise);
		}
	}

	return values;
}






