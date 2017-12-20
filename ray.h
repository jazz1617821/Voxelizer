#ifndef _RAY_H
#define _RAY_H

#include <stdbool.h>

void getRayDir(const int mouseX, const int mouseY,
			const int w, const int height,
			const float viewMat[16], const float proMat[16], float dir[3]);

bool shootTriangle(float pos[3], float dir[3], float vertex[9], float proj[3]);

bool projPointTriangle(float projPnt[3], float vertex[9], float nomral[3]);

#endif