#ifndef _IMAGE_H
#define _IMAGE_H

#include <Windows.h>
#include <stdbool.h>

int loadBMP(char *filename, int *w, int *h, unsigned char **data);
void saveBMP(char *filename, int w, int h, unsigned char *data);
float biLinear(float v[4], float x, float y);
float triLinear(float v[8], float x, float y, float z);

#endif // !_IMAGE_H
