#ifndef _IMAGE3D_H
#define _IMAGE3D_H

#include "voxelize.h"

void voxelDilation(VoxelData *vdata, int *mask, int maskSize, int maskValue);
void voxelErosion(VoxelData *vdata, int *mask, int maskSize, int maskValue);
void voxelDilation3X3X3(VoxelData *vdata, int maskValue);
void voxelDilationCity3(VoxelData *vdata, int maskValue);
void voxelErosion3X3X3(VoxelData *vdata, int maskValue);
void voxelErosionCity3(VoxelData *vdata, int maskValue);

#endif // _IMAGE3D_H
