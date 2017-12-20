#include "light.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

Light *newLight(const char *name)
{
	Light *light;

	light = (Light*)calloc(1, sizeof(Light));
	if (light == NULL) {
		assert("Null...\n");
	}
	strcpy(light->name, name);
	setLightPosition(light, 1, 1, 1, 0);
	setLightDirection(light, -1, -1, -1);
	setLightDiffuse(light, 0.6, 0.6, 0.6, 1.0);
	setLightSpecular(light, 1.0, 1.0, 1.0, 1.0);
	setLightEmission(light, 0.0, 0.0, 0.0, 0.0);
	setLightCutOff(light, 90.0);

	return light;
}

void deleteLight(Light **light)
{
	free((*light));
	*light = NULL;
}

GlobalLight *newGlobalLight(void)
{
	GlobalLight *glight;

	glight = (GlobalLight*)calloc(1, sizeof(GlobalLight));
	if (glight == NULL) {
		assert("Null...\n");
	}

	setLightAmbient(glight, 0.22, 0.22, 0.22, 1.0);

	return glight;
}

void deleteGlobalLight(GlobalLight  **glight)
{
	free((*glight));
	*glight = NULL;
}

void setLightPosition(Light *light, float x, float y, float z, float w)
{
	light->position[0] = x;
	light->position[1] = y;
	light->position[2] = z;
	light->position[3] = w;
}

void setLightDirection(Light *light, float x, float y, float z)
{
	light->direction[0] = x;
	light->direction[1] = y;
	light->direction[2] = z;
}

void setLightDiffuse(Light *light, float r, float g, float b, float a)
{
	light->diffuse[0] = r;
	light->diffuse[1] = g;
	light->diffuse[2] = b;
	light->diffuse[3] = a;
}

void setLightSpecular(Light *light, float r, float g, float b, float a)
{
	light->specular[0] = r;
	light->specular[1] = g;
	light->specular[2] = b;
	light->specular[3] = a;
}

void setLightEmission(Light *light, float r, float g, float b, float a)
{
	light->emission[0] = r;
	light->emission[1] = g;
	light->emission[2] = b;
	light->specular[3] = a;
}

void setLightCutOff(Light *light, float angles)
{
	light->cutoff = angles;
}

void setLightAmbient(GlobalLight *glight, float r, float g, float b, float a)
{
	glight->ambient[0] = r;
	glight->ambient[1] = g;
	glight->ambient[2] = b;
	glight->ambient[3] = a;
}
