#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <stdbool.h>

/*_______________________________________________________________________________
field graphic Material function
-------------------------------
fgMaterial:
setup the model material. associate with lighting effect.
Following is material example.

---------------------------------------------------------------------------------------------------------
Name            Ambient(Ka)                Diffuse(Kd)                Specular(Ks)               Shinines
---------------------------------------------------------------------------------------------------------
black plastic   .0       .0       .0       .01      .01      .01      .50      .50      .50      .25
black rubber    .02      .02      .02      .01      .01      .01      .4       .4       .4       .078125
brass           .329412  .223529  .027451  .780392  .568627  .113725  .992157  .941176  .807843  .217948
bronze          .2125    .1275    .054     .714     .4284    .18144   .393548  .271906  .166721  .2
chrome          .25      .25      .25      .4       .4       .4       .774597  .774597  .774597  .6
copper          .19125   .0735    .0225    .7038    .27048   .0828    .256777  .137622  .086014  .1
cyan plastic    .0       .1       .06      .0       .5098039 .5098039 .5019607 .5019607 .5019607 .25
cyan rubber     .0       .05      .05      .4       .5       .5       .04      .7       .7       .078125
emerald         .0215    .1745    .0215    .07568   .61424   .07568   .633     .727811  .633     .6
gold            .24725   .1995    .0745    .75164   .60648   .22648   .628281  .555802  .366065  .4
green plastic   .0       .0       .0       .1       .35      .1       .45      .55      .45      .25
green rubber    .0       .05      .0       .4       .5       .4       .04      .7       .04      .078125
jade            .135     .2225    .1575    .54      .89      .63      .316228  .316228  .316228  .1
obsidian        .05375   .05      .06625   .18275   .17      .22525   .332741  .328634  .346435  .3
pearl           .25      .20725   .20725   1         .829     .829     .296648  .296648  .296648  .088
red plastic     .0       .0       .0       .5       .0       .0       .7       .6       .6       .25
red rubber      .05      .0       .0       .5       .4       .4       .7       .04      .04      .078125
ruby            .1745    .01175   .01175   .61424   .04136   .04136   .727811  .626959  .626959  .6
silver          .19225   .19225   .19225   .50754   .50754   .50754   .508273  .508273  .508273  .4
turquoise       .1       .18725   .1745    .396     .74151   .69102   .297254  .30829   .306678  .1
white plastic   .0       .0       .0       .55      .55      .55      .70      .70      .70      .25
white rubber    .05      .05      .05      .5       .5       .5       .7       .7       .7       .078125
yellow plastic  .0       .0       .0       .5       .5       .0       .60      .60      .50      .25
yellow rubber   .05      .05      .0       .5       .5       .4       .7       .7       .04      .078125
---------------------------------------------------------------------------------------------------------
ps: Multiply the shininess by 128!

_______________________________________________________________________________*/


enum MTL_TYPE 
{
	MTL_BLACK_PLASTIC,
	MTL_BLACK_RUBBER,
	MTL_BRASS,
	MTL_BRONZE,
	MTL_CHROME,
	MTL_COPPER,
	MTL_CYAN_PLASTIC,
	MTL_CYAN_RUBBER,
	MTL_EMERALD,
	MTL_GOLD,
	MTL_GREEN_PLASTIC,
	MTL_GREEN_RUBBER,
	MTL_JADE,
	MTL_OBSIDIAN,
	MTL_PEARL,
	MTL_RED_PLASTIC,
	MTL_RED_RUBBER,
	MTL_RUBY,
	MTL_SILVER,
	MTL_TURQUOISE,
	MTL_WHITE_PLASTIC,
	MTL_WHITE_RUBBER,
	MTL_YELLOW_PLASTUC,
	MTL_YELLOW_RUBBER,
	NUM_MATERIALS
};

typedef struct Material{
	char name[100];
	float ambient[4];
	float specular[4];
	float diffuse[4];
	float shininess;
	bool isEnable;
}Material;

Material *newMaterial(void);
Material *newDefaultMaterial(int type);
void deleteMaterial(Material **mtl);
void setMaterialAmbient(Material *mtl, float r, float g, float b, float a);
void setMaterialSpecular(Material *mtl, float r, float g, float b, float a);
void setMaterialDiffuse(Material *mtl, float r, float g, float b, float a);
void setMaterialShininess(Material *mtl, float shininess);

// file
void writeMaterial(Material *mtl);
Material *readMaterial(const char* fileName);

#endif