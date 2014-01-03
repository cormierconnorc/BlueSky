/*
 * Perlin.h
 *
 *  Created on: May 11, 2013
 *      Author: Connor Cormier
 */

#ifndef PERLIN_H_
#define PERLIN_H_

float findNoise2D(float, float), interpolate(float, float, float), getNoise(float, float);
std::vector<std::vector<float> > render(int, int, float, float);


#endif /* PERLIN_H_ */
