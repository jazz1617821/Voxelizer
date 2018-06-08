#pragma once
#ifndef _RAWFILE_H
#define _RAWFILE_H
#include "voxel.h"
#include <stdio.h>

void saveRaw(const char*, VoxelData*);
void writeInfoFile(const char*, VoxelData*);
void writeRawFile(const char*, VoxelData*);

void* readRaw(const char*);
void readInfoFile(const char*);
float* readRawFileFloat(const char*);
unsigned char* readRawFileUC(const char*);

#endif // !_RAWFILE_H
