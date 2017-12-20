#include "image3D.h"
#include <stdbool.h>

#define X 0
#define Y 1
#define Z 2

#define MASK_3X3X3_SIZE 26
#define MASK_CITY_3_SIZE 6

const int mask3x3x3[MASK_3X3X3_SIZE][3] =
{
	{ -1, -1, -1 },{ 0, -1, -1 },{ 1, -1, -1 },
	{ -1,  0, -1 },{ 0,  0, -1 },{ 1,  0, -1 },
	{ -1,  1, -1 },{ 0,  1, -1 },{ 1,  1, -1 },

	{ -1, -1,  0 },{ 0, -1,  0 },{ 1, -1,  0 },
	{ -1,  0,  0 },				 { 1,  0,  0 },
	{ -1,  1,  0 },{ 0,  1,  0 },{ 1,  1,  0 },

	{ -1, -1,  1 },{ 0, -1,  1 },{ 1, -1,  1 },
	{ -1,  0,  1 },{ 0,  0,  1 },{ 1,  0,  1 },
	{ -1,  1,  1 },{ 1,  1,  1 },{ 1,  1,  1 }
};

const int maskCity3[MASK_CITY_3_SIZE][3] =
{
				   { 0,  0, -1 },
				   { 0, -1,  0 },
	{ -1,  0,  0 },				 { 1,  0,  0 },
				   { 0,  1,  0 },
				   { 0,  0,  1 }
};

void voxelDilation(VoxelData *vdata, int *mask, int maskSize, int maskValue)
{
	int i, j, k, l, x, y, z, index, neighbor, num, total;

	for (i = 0; i < vdata->resolution[Z]; ++i)
	{
		for (j = 0; j < vdata->resolution[Y]; ++j)
		{
			for (k = 0; k < vdata->resolution[X]; ++k)
			{
				index = k + j * vdata->resolution[X] + i * vdata->resolution[X] * vdata->resolution[Y];
				// set mask voxel
				if (vdata->rawData[index].data == maskValue) {
					for (l = 0; l < maskSize; ++l)
					{
						x = k + *(mask + l * 3);
						y = j + *(mask + l * 3 + 1);
						z = i + *(mask + l * 3 + 2);
						// out of bound
						if (x < 0 || x >= vdata->resolution[X] ||
							y < 0 || y >= vdata->resolution[Y] ||
							z < 0 || z >= vdata->resolution[Z])
							continue;
						neighbor = x + y * vdata->resolution[X] + z * vdata->resolution[X] * vdata->resolution[Y];
						if (vdata->rawData[neighbor].data == VOX_EMPTY) {
							vdata->rawData[neighbor].data = VOX_UNKNOWN;
						}
					} // end mask 
				}
			} // end x
		} // end y
	} // end z

	num = 0;
	total = vdata->resolution[X] * vdata->resolution[Y] * vdata->resolution[Z];
	for (i = 0; i < total; ++i)
	{
		if (vdata->rawData[i].data == VOX_UNKNOWN) {
			vdata->rawData[i].data = maskValue;
			num++;
		}
	}
	printf("num: %d\n", num);
}

void voxelErosion(VoxelData *vdata, int *mask, int maskSize, int maskValue)
{
	int i, j, k, l, x, y, z, index, neighbor, num, total;

	for (i = 0; i < vdata->resolution[Z]; ++i)
	{
		for (j = 0; j < vdata->resolution[Y]; ++j)
		{
			for (k = 0; k < vdata->resolution[X]; ++k)
			{
				index = k + j * vdata->resolution[X] + i * vdata->resolution[X] * vdata->resolution[Y];
				// bound process
				if (k == 0 || k == vdata->resolution[X] - 1 ||
					j == 0 || j == vdata->resolution[Y] - 1 ||
					i == 0 || i == vdata->resolution[Z] - 1) {
					if (vdata->rawData[index].data == maskValue) {
						vdata->rawData[index].data = VOX_UNKNOWN;
						continue;
					}
				}
				// check mask voxel
				if (vdata->rawData[index].data == maskValue) {
					for (l = 0; l < maskSize; ++l)
					{
						x = k + *(mask + l * 3);
						y = j + *(mask + l * 3 + 1);
						z = i + *(mask + l * 3 + 2);
						neighbor = x + y * vdata->resolution[X] + z * vdata->resolution[X] * vdata->resolution[Y];
						if (vdata->rawData[neighbor].data != maskValue || 
							vdata->rawData[neighbor].data != VOX_UNKNOWN) {
							vdata->rawData[neighbor].data = VOX_UNKNOWN;
						}
					} // end mask 
				}
			} // end x
		} // end y
	} // end z

	num = 0;
	total = vdata->resolution[X] * vdata->resolution[Y] * vdata->resolution[Z];
	for (i = 0; i < total; ++i)
	{
		if (vdata->rawData[i].data == VOX_UNKNOWN) {
			vdata->rawData[i].data = VOX_EMPTY;
			num++;
		}
	}
	printf("num: %d\n", num);
}

void voxelDilation3X3X3(VoxelData *vdata, int maskValue)
{
	voxelDilation(vdata, mask3x3x3, MASK_3X3X3_SIZE, maskValue);
}

void voxelDilationCity3(VoxelData *vdata, int maskValue)
{
	voxelDilation(vdata, maskCity3, MASK_CITY_3_SIZE, maskValue);
}

void voxelErosion3X3X3(VoxelData *vdata, int maskValue)
{
	voxelErosion(vdata, mask3x3x3, MASK_3X3X3_SIZE, maskValue);
}

void voxelErosionCity3(VoxelData *vdata, int maskValue)
{
	voxelErosion(vdata, maskCity3, MASK_CITY_3_SIZE, maskValue);
}