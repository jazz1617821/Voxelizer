#ifndef _BOUNDING_BOX_H
#define _BOUNDING_BOX_H

#include <stdbool.h>
#include "vbo.h"

// axis aligned bounding box
typedef struct AABB
{
	float min[3];
	float max[3];

	// render staff
	VBO *vbo;
}AABB;


// bounding sphere
typedef struct BS
{
	float center[3];
	float radius;

	// render staff
	VBO *vbo;
}BS;

// aabb
AABB *newAABB(void);
void deleteAABB(AABB **bbox);
void initAABB(AABB *bbox);
bool addBound(AABB *bbox, float val[3]);
bool addAABB(AABB *des, AABB *src);
void setAABB(AABB *dest, AABB *src);
void aabbCenter(AABB bbox, float center[3]);
void aabbSize(AABB bbox, float size[3]);
float aabbMaxSize(AABB bbox);
void transformAABB(AABB *bbox, Transformation *tsfm);

// bs
BS *newBS(void);
void deleteBS(BS **bs);
void initBS(BS *bs);
void transformBS(BS *bs, Transformation *tsfm);

#endif // !_BOUNDING_BOX_H
