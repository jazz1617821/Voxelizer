#include "material.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <Windows.h>

char *g_mtlNames[NUM_MATERIALS] =
{
	"black_plastic",
	"black_rubber",
	"brass",
	"bronze",
	"chrome",
	"copper",
	"cyan_plastic",
	"cyan_rubber",
	"emerald",
	"gold",
	"green_plastic",
	"green_rubber",
	"jade",
	"obsidian",
	"pearl",
	"red_plastic",
	"red_rubber",
	"ruby",
	"silver",
	"turquoise",
	"white_plastic",
	"white_rubber",
	"yellow_plastic",
	"yellow_rubber"
};

float g_mtlAmbients[NUM_MATERIALS][3] =
{
	{ 0.0       , 0.0       , 0.0 },
	{ 0.02      , 0.02      , 0.02 },
	{ 0.329412  , 0.223529  , 0.027451 },
	{ 0.2125    , 0.1275    , 0.054 },
	{ 0.25      , 0.25      , 0.25 },
	{ 0.19125   , 0.0735    , 0.0225 },
	{ 0.0       , 0.1       , 0.06 },
	{ 0.0       , 0.05      , 0.05 },
	{ 0.0215    , 0.1745    , 0.0215 },
	{ 0.24725   , 0.1995    , 0.0745 },
	{ 0.0       , 0.0       , 0.0 },
	{ 0.0       , 0.05      , 0.0 },
	{ 0.135     , 0.2225    , 0.1575 },
	{ 0.05375   , 0.05      , 0.06625 },
	{ 0.25      , 0.20725   , 0.20725 },
	{ 0.0       , 0.0       , 0.0 },
	{ 0.05      , 0.0       , 0.0 },
	{ 0.1745    , 0.01175   , 0.01175 },
	{ 0.19225   , 0.19225   , 0.19225 },
	{ 0.1       , 0.18725   , 0.1745 },
	{ 0.0       , 0.0       , 0.0 },
	{ 0.05      , 0.05      , 0.05 },
	{ 0.0       , 0.0       , 0.0 },
	{ 0.05      , 0.05      , 0.0 },
};

float g_mtlDiffuses[NUM_MATERIALS][3] =
{
	{ 0.01      , 0.01      , 0.01 },
	{ 0.01      , 0.01      , 0.01 },
	{ 0.780392  , 0.568627  , 0.113725 },
	{ 0.714     , 0.4284    , 0.18144 },
	{ 0.4       , 0.4       , 0.4 },
	{ 0.7038    , 0.27048   , 0.0828 },
	{ 0.0       , 0.5098039 , 0.5098039 },
	{ 0.4       , 0.5       , 0.5 },
	{ 0.07568   , 0.61424   , 0.07568 },
	{ 0.75164   , 0.60648   , 0.22648 },
	{ 0.1       , 0.35      , 0.1 },
	{ 0.4       , 0.5       , 0.4 },
	{ 0.54      , 0.89      , 0.63 },
	{ 0.18275   , 0.17      , 0.22525 },
	{ 1        , 0.829     , 0.829 },
	{ 0.5       , 0.0       , 0.0 },
	{ 0.5       , 0.4       , 0.4 },
	{ 0.61424   , 0.04136   , 0.04136 },
	{ 0.50754   , 0.50754   , 0.50754 },
	{ 0.396     , 0.74151   , 0.69102 },
	{ 0.55      , 0.55      , 0.55 },
	{ 0.5       , 0.5       , 0.5 },
	{ 0.5       , 0.5       , 0.0 },
	{ 0.5       , 0.5       , 0.4 }
};

float g_mtlSpeculars[NUM_MATERIALS][3] = 
{
	{ 0.50      , 0.50      , 0.50},
	{ 0.4       , 0.4       , 0.4 },
	{ 0.992157  , 0.941176  , 0.807843 },
	{ 0.393548  , 0.271906  , 0.166721 },
	{ 0.774597  , 0.774597  , 0.774597 },
	{ 0.256777  , 0.137622  , 0.086014 },
	{ 0.5019607 , 0.5019607 , 0.5019607 },
	{ 0.04      , 0.7       , 0.7 },
	{ 0.633     , 0.727811  , 0.633 },
	{ 0.628281  , 0.555802  , 0.366065 },
	{ 0.45      , 0.55      , 0.45 },
	{ 0.04      , 0.7       , 0.04 },
	{ 0.316228  , 0.316228  , 0.316228 },
	{ 0.332741  , 0.328634  , 0.346435 },
	{ 0.296648  , 0.296648  , 0.296648 },
	{ 0.7       , 0.6       , 0.6 },
	{ 0.7       , 0.04      , 0.04 },
	{ 0.727811  , 0.626959  , 0.626959 },
	{ 0.508273  , 0.508273  , 0.508273 },
	{ 0.297254  , 0.30829   , 0.306678 },
	{ 0.70      , 0.70      , 0.70 },
	{ 0.7       , 0.7       , 0.7 },
	{ 0.60      , 0.60      , 0.50 },
	{ 0.7       , 0.7       , 0.04 }
};

float g_mtlShininesses[NUM_MATERIALS] =
{
	0.25,
	0.078125,
	0.217948,
	0.2,
	0.6,
	0.1,
	0.25,
	0.078125,
	0.6,
	0.4,
	0.25,
	0.078125,
	0.1,
	0.3,
	0.088,
	0.25,
	0.078125,
	0.6,
	0.4,
	0.1,
	0.25,
	0.078125,
	0.25,
	0.078125
};


Material *newMaterial(void)
{
	Material *mtl;
	
	mtl = (Material*)calloc(1, sizeof(Material));
	if (mtl == NULL) {
		assert("NULL...\n");
	}
	setMaterialAmbient(mtl, .1, .18725, .1745, 1.0);
	setMaterialDiffuse(mtl, .396, .74151, .69102, 1.0);
	setMaterialSpecular(mtl, .297254, .30829, .306678, 1.0);
	setMaterialShininess(mtl, 0.1 * 128.0);
	mtl->isEnable = true;

	return mtl;
}

Material *newDefaultMaterial(int type)
{
	Material *mtl;

	mtl = (Material*)calloc(1, sizeof(Material));
	if (mtl == NULL) {
		assert("NULL...\n");
	}
	
	strcpy(mtl->name, g_mtlNames[type]);
	memcpy(mtl->ambient, g_mtlAmbients[type], 3 * sizeof(float));
	memcpy(mtl->diffuse, g_mtlDiffuses[type], 3 * sizeof(float));
	memcpy(mtl->specular, g_mtlSpeculars[type], 3 * sizeof(float));
	memcpy(&mtl->shininess, &g_mtlShininesses[type], 1 * sizeof(float));

	return mtl;
}

void deleteMaterial(Material **mtl)
{
	free((*mtl));
	*mtl = NULL;
}

void setMaterialAmbient(Material *mtl, float r, float g, float b, float a)
{
	mtl->ambient[0] = r;
	mtl->ambient[1] = g;
	mtl->ambient[2] = b;
	mtl->ambient[3] = a;
}

void setMaterialSpecular(Material *mtl, float r, float g, float b, float a)
{
	mtl->specular[0] = r;
	mtl->specular[1] = g;
	mtl->specular[2] = b;
	mtl->specular[3] = a;
}

void setMaterialDiffuse(Material *mtl, float r, float g, float b, float a)
{
	mtl->diffuse[0] = r;
	mtl->diffuse[1] = g;
	mtl->diffuse[2] = b;
	mtl->diffuse[3] = a;
}

void setMaterialShininess(Material *mtl, float shininess)
{
	mtl->shininess = shininess;
}

void spaceToUnderline(char *str)
{
	char *ptr;

	ptr = strchr(str, ' ');
	while (ptr != NULL)
	{
		ptr[0] = '_';
		ptr = strchr(str, ' ');
	}
}

void writeMaterial(Material *mtl)
{
	FILE* fp;
	char buffer[2048], name[2048], path[2048];

	GetCurrentDirectoryA(1024, path);
	strcat(path, "\\materials\\");
	if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(path, 0);
	}

	spaceToUnderline(mtl->name);
	strcpy(name, mtl->name);
	strcpy(buffer, path);
	strcat(buffer, name);
	strcat(buffer, ".mtl503");
	fp = fopen(buffer, "wb");
	fwrite(mtl->name, sizeof(char), 100, fp);
	fwrite(&mtl->ambient, sizeof(float), 3, fp);
	fwrite(&mtl->diffuse, sizeof(float), 3, fp);
	fwrite(&mtl->specular, sizeof(float), 3, fp);
	fwrite(&mtl->shininess, sizeof(float), 1, fp);
	fclose(fp);
}

Material *readMaterial(const char *fileName)
{
	Material *mtl;
	FILE* fp;

	mtl = newMaterial();

	fp = fopen(fileName, "rb");
	fread(mtl->name, sizeof(char), 100, fp);
	fread(&mtl->ambient, sizeof(float), 3, fp);
	fread(&mtl->diffuse, sizeof(float), 3, fp);
	fread(&mtl->specular, sizeof(float), 3, fp);
	fread(&mtl->shininess, sizeof(float), 1, fp);
	fclose(fp);

	return mtl;
}