#pragma once
#ifndef _RAWFILE_H
#define _RAWFILE_H
#include "voxel.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void saveRaw(const char*, VoxelData*);
void writeInfoFile(const char*, VoxelData*);
void writeRawFile(const char*, VoxelData*);

#endif // !_RAWFILE_H
