#ifndef _LIGHT_H
#define _LIGHT_H

#include <stdbool.h>

#define LIGHT_NAME_SIZE 2048

typedef struct Light {
	char name[LIGHT_NAME_SIZE];
	float position[4];
	float direction[3];
	float diffuse[4];
	float specular[4];
	float emission[4];
	float cutoff;
}Light;

typedef struct GlobalLight {
	float ambient[4];
}GlobalLight;

Light *newLight(const char *name);
void deleteLight(Light **light);
GlobalLight* newGlobalLight(void);
void deleteGlobalLight(GlobalLight **glight);
void setLightPosition(Light *light, float x, float y, float z, float w);
void setLightDirection(Light *light, float x, float y, float z);
void setLightDiffuse(Light *light, float r, float g, float b, float a);
void setLightSpecular(Light *light, float r, float g, float b, float a);
void setLightEmission(Light *light, float r, float g, float b, float a);
void setLightCutOff(Light *light, float angles);
void setLightAmbient(GlobalLight *glight, float r, float g, float b, float a);

#endif