#ifndef _MODELVBO_H
#define _MODELVBO_H

#include "meshmodel.h"
#include "vbo.h"
#include <stdbool.h>

VBO *createModelVBO(MeshModel *model);
void setModelID(MeshModel *model, VBO *vbo);
void setModelFaceID(MeshModel *model, VBO *vbo);

#endif // !_MODELVBO_H
