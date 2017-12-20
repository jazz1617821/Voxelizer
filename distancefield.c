#include "distancefield.h"
#include "mymath.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define X 0
#define Y 1
#define Z 2

#define SQRT3 1.7320508
#define SQRT2 1.4142135


int g_twoPassForwardMat3[13][3] = 
{
	{ -1, -1, -1 }, {  0, -1, -1 }, {  1, -1, -1 },
	{ -1,  0, -1 }, {  0,  0, -1 }, {  1,  0, -1 },
	{ -1,  1, -1 }, {  0,  1, -1 }, {  1,  1, -1 },

	{ -1, -1,  0 }, {  0, -1,  0 }, {  1, -1,  0 },
	{ -1,  0,  0 }
};

int g_twoPassBackwardMat3[13][3] =
{
	                                {  1,  0,  0 },
	{ -1,  1,  0 }, {  0,  1,  0 }, {  1,  1,  0 },

	{ -1, -1,  1 }, {  0, -1,  1 }, {  1, -1,  1 },
	{ -1,  0,  1 }, {  0,  0,  1 }, {  1,  0,  1 },
	{ -1,  1,  1 }, {  1,  1,  1 }, {  1,  1,  1 }
};

float chessboardMat3[26] = 
{ 
	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,

	1.0, 1.0, 1.0,
	1.0,	  1.0,
	1.0, 1.0, 1.0,

	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0
};

float euclideanMat3[26] = 
{
	SQRT3, SQRT2, SQRT3,
	SQRT2,     1, SQRT2,
	SQRT3, SQRT2, SQRT3,

	SQRT2,     1, SQRT2,
	    1,            1,
	SQRT2,     1, SQRT2,

	SQRT3, SQRT2, SQRT3,
	SQRT2,     1, SQRT2,
	SQRT3, SQRT2, SQRT3
};



static void cdt3X3(DistanceField *df, float *mat);

CDDList *newCDDList(int tri, float dist)
{
	CDDList *ls;

	ls = (CDDList*)calloc(1, sizeof(CDDList));
	initEnqDList(&ls->link);
	ls->cdd.tri = tri;
	ls->cdd.dist = dist;

	return ls;
}

CDDList **newCDDListArray(int dfRes[3], float dfSize[3], int bound[6], VoxelData *vdata)
{
	int i, j, k, total, dfIdx, voxIdx;
	float boxCenter[3], vertices[9], dist, proj[3];
	CDDList **lsArray, *ls;
	TriVoxel *trivox, *tri;
	
	trivox = vdata->lsArray;
	total = dfRes[X] * dfRes[Y] * dfRes[Z];
	lsArray = (CDDList**)calloc(total, sizeof(CDDList*));

	// initialze cdd list
	// tri = -1
	// distance = infinite
	for (i = 0; i < total; ++i)
	{
		lsArray[i] = newCDDList(-1, INT_MAX);
	}

	// copy triangle list and calculate the distance
	for (i = 0; i < vdata->resolution[Z]; ++i)
	{
		for (j = 0; j < vdata->resolution[Y]; ++j)
		{
			for (k = 0; k < vdata->resolution[X]; ++k)
			{
				voxIdx = k + j * vdata->resolution[X] * i * vdata->resolution[X] * vdata->resolution[Y];
				dfIdx = (k + bound[DF_LEFT]) +
						(j + bound[DF_BOTTOM]) * dfRes[X] +
						(i + bound[DF_BACK]) * dfRes[X] * dfRes[Y];
				while (!isEmptyEnq(&trivox[voxIdx].item))
				{
					tri = (TriVoxel*)removeEnqHead(&trivox[voxIdx].item);
					getTriangle(vdata->model, tri, vertices);
					
					// wait for the next time...
					// calculate the distances

					ls = newCDDList(tri->index, dist);
					addEnqTail(&lsArray[dfIdx]->link, &ls->link);
				}
			}
		}
	}

	return lsArray;
}

void deleteCDDListArray(CDDList **lsArray)
{

}

// no cdfr
// create or reallocate an distance field
DistanceField *newDistanceField(int dfRes[3], float dfSize[3], int bound[6], DistanceField *reusage)
{
	int total;
	DistanceField *df;

	if (reusage != NULL) {
		df = reusage;
		initAABB(df->bbox);
	}
	else {
		df = (DistanceField*)calloc(1, sizeof(DistanceField));
		df->bbox = newAABB();
	}

	// set variables
	memcpy(df->resolution, dfRes, 3 * sizeof(int));
	memcpy(df->voxelSize, dfSize, 3 * sizeof(float));
	memcpy(df->bound, bound, 6 * sizeof(int));
	df->minValue = INT_MAX;
	df->maxValue = INT_MIN;
	
	// allocate memory and set all data into INT_MAX
	total = dfRes[X] * dfRes[Y] * dfRes[Z];
	if (reusage == NULL)
		df->data = (float*)calloc(total, sizeof(float));
	else
		df->data = (float*)realloc(df->data, total * sizeof(float));

	return df;
}

void deleteDistanceField(DistanceField **df)
{
	DistanceField *del;
	
	del = (*df);

	if (del == NULL)
		return;

	deleteAABB(&del->bbox);
	free(del->data);
	//deleteCDDListArray(del->lsArray);
	deleteVBO(&del->vbo);

	free(del);
	*df = NULL;
}

void initDistanceField(DistanceField *df, VoxelObject *obj)
{
	int i, j, k, total, dfIdx, voxIdx;
	VoxelData *vdata;

	vdata = obj->dataItem->vdata;
	setAABB(df->bbox, obj->bbox);
	df->bbox->min[X] -= (df->bound[DF_LEFT] * df->voxelSize[X]);
	df->bbox->min[Y] -= (df->bound[DF_BOTTOM] * df->voxelSize[Y]);
	df->bbox->min[Z] -= (df->bound[DF_BACK] * df->voxelSize[Z]);
	df->bbox->max[X] += (df->bound[DF_RIGHT] * df->voxelSize[X]);
	df->bbox->max[X] += (df->bound[DF_TOP] * df->voxelSize[Y]);
	df->bbox->max[X] += (df->bound[DF_FRONT] * df->voxelSize[Z]);

	/*
	// cdfr
	if (vdata->lsArray != NULL) {
		df->lsArray = newCDDListArray(dfRes, bound, vdata->resolution, vdata->lsArray);
	}
	*/

	// initialize df
	// if empty = infinite (INI_MAX)
	// if surface = 0
	// if solid = -infinite (INT_MIN)
	// origin [ left(x = 0) bottom (y = 0) back(z = 0) ]
	for (i = 0; i < vdata->resolution[Z]; ++i)
	{
		for (j = 0; j < vdata->resolution[Y]; ++j)
		{
			for (k = 0; k < vdata->resolution[X]; ++k)
			{
				voxIdx = k + j * vdata->resolution[X] + i * vdata->resolution[X] * vdata->resolution[Y];
				if (vdata->rawData[voxIdx].data != VOX_EMPTY) {
					dfIdx = (k + df->bound[DF_LEFT]) +
						(j + df->bound[DF_BOTTOM]) * df->resolution[X] +
						(k + df->bound[DF_BACK]) * df->resolution[X] * df->resolution[Y];
					if (vdata->rawData[voxIdx].data == VOX_SURFACE) {
						df->data[dfIdx] = 0;
					}
					if (vdata->rawData[voxIdx].data == VOX_SOLID) {
						df->data[dfIdx] = INT_MIN;
					}
				}
			} // end x
		} // end y
	} // end z
}

void updateDF(DistanceField *df, int center, int x, int y, int z, float delta)
{
	int index;
	float nbDist;
	
	if (x < 0 || x >= df->resolution[X] ||
		y < 0 || y >= df->resolution[Y] ||
		z < 0 || z >= df->resolution[Z]) {
		nbDist = INT_MAX;
	}
	else {
		// check the sign of distance
		nbDist = fabs(df->data[x +
							   y * df->resolution[X] +
							   z * df->resolution[X] * df->resolution[Y]]);
	}

	// add mask value
	if (nbDist != INT_MAX || nbDist != INT_MIN)
		nbDist += delta;

	// find the min distance
	// check the sign of distance
	if (nbDist < fabs(df->data[center])) {
		if (df->data[center] < 0)
			df->data[center] = -nbDist;
		else
			df->data[center] = nbDist;
	}
}

void cdt3X3(DistanceField *df, float *mat)
{
	int i, j, k, l, index, total;

	// first pass
	for (i = 0; i < df->resolution[Z]; ++i)
	{
		for (j = 0; j < df->resolution[Y]; ++j)
		{
			for (k = 0; k < df->resolution[X]; ++k)
			{
				index = k +
						j * df->resolution[X] +
						i * df->resolution[X] * df->resolution[Y];
				for (l = 0; l < 13; ++l)
				{
					updateDF(df,
							 index,
						 	 k + g_twoPassForwardMat3[l][X],
							 j + g_twoPassForwardMat3[l][Y],
							 i + g_twoPassForwardMat3[l][Z],
							 mat[l]);
				} // end loop l
			} // end loop k
		} // end loop j
	} // end loop i

	// second pass
	for (i = df->resolution[Z] - 1; i >= 0; --i)
	{
		for (j = df->resolution[Y] - 1; j >= 0; --j)
		{
			for (k = df->resolution[X] - 1; k >= 0; --k)
			{
				index = k +
					j * df->resolution[X] +
					i * df->resolution[X] * df->resolution[Y];
				for (l = 0; l < 13; ++l)
				{
					updateDF(df,
							 index,
							 k + g_twoPassBackwardMat3[l][X],
							 j + g_twoPassBackwardMat3[l][Y],
							 i + g_twoPassBackwardMat3[l][Z],
							 mat[13 + l]);
				} // end loop l
			} // end loop k
		} // end loop j
	} // end loop i

	// find max and min distance values
	total = df->resolution[X] * df->resolution[Y] * df->resolution[Z];
	for (i = 0; i < total; ++i)
	{
		if (df->maxValue < df->data[i])
			df->maxValue = df->data[i];
		
		if (df->minValue > df->data[i])
			df->minValue = df->data[i];
	}

#ifdef _DEBUG
	printf("DF max: %f\n", df->maxValue);
	printf("DF min: %f\n", df->minValue);
#endif // _DEBUG


}

void CDT(DistanceField *df, VoxelObject *obj, int type)
{
	int i, total;
	DT_TYPE dtType;

	dtType = (DT_TYPE)type;
	df->type = type;

#ifdef _DEBUG
	printf("DF initialize start\n");
#endif // _DEBUG

	// init
	initDistanceField(df, obj);

#ifdef _DEBUG
	printf("DF initialize done...\n");
#endif // _DEBUG


#ifdef _DEBUG
	printf("DF start\n");
#endif // _DEBUG
	switch (dtType)
	{
	case DT_CHAMFER:
		break;
	case DT_CHESSBOARD:
		cdt3X3(df, chessboardMat3);
		break;
	case DT_CITYBLOCK:
		break;
	case DT_EUCLIDEAN:
		cdt3X3(df, euclideanMat3);
		break;
	default:
		break;
	}
#ifdef _DEBUG
	printf("DF done...\n");
#endif // _DEBUG
}


float dataValue(int x, int y, int z, DistanceField *df)
{	
	return df->data[x +
					y * df->resolution[X] +
					z * df->resolution[X] * df->resolution[Y]];
}


float pntTriDist(const float pnt[3], const float tri[9], float projPnt[3])
{
	float P0[3] = { pnt[0], pnt[1], pnt[2] };
	float P1[3] = { tri[0], tri[1], tri[2] };
	float P2[3] = { tri[3], tri[4], tri[5] };
	float P3[3] = { tri[6], tri[7], tri[8] };
	float tmp0[3], tmp1[3], tmp2[3], Np[3], P0p[3], P0pp[3], P0_P0p[3], P0p_P0pp[3], V1[3], V2[3], V3[3], R[3], param[4];
	int sign;
	float dist, distp, f1, f2, f3, cosAlpha, cosGamma, t, d1, d2;

	subtract3fv(P2, P1, tmp0);
	subtract3fv(P3, P1, tmp1);
	cross3fv(tmp0, tmp1, Np);
	normalize3fv(Np);

	compPlane(Np, P1, param);
	sign = compPointPlane(param, P0);
	if (sign == 0) sign = 1;
	
	subtract3fv(P0, P1, tmp0);
	cosAlpha = dot3fv(tmp0, Np) / (length3fv(tmp0) * length3fv(Np));
	subtract3fv(P1, P0, tmp0);
	dist = length3fv(tmp0) * cosAlpha;
	dist = fabs(dist);
	tmp0[0] = tmp0[1] = tmp0[2] = -sign * dist;
	mult3fv(tmp0, Np, P0_P0p);
	add3fv(P0, P0_P0p, P0p);	// the projection point

	subtract3fv(P1, P2, tmp0);
	normalize3fv(tmp0);
	subtract3fv(P1, P3, tmp1);
	normalize3fv(tmp1);
	add3fv(tmp0, tmp1, V1);		//vector from center of triangle to P1

	subtract3fv(P2, P3, tmp0);
	normalize3fv(tmp0);
	subtract3fv(P2, P1, tmp1);
	normalize3fv(tmp1);
	add3fv(tmp0, tmp1, V2);		//vector from center of triangle to P2

	subtract3fv(P3, P1, tmp0);
	normalize3fv(tmp0);
	subtract3fv(P3, P2, tmp1);
	normalize3fv(tmp1);
	add3fv(tmp0, tmp1, V3);		//vector from center of triangle to P3

	subtract3fv(P0p, P1, tmp0);
	cross3fv(V1, tmp0, tmp1);
	f1 = dot3fv(tmp1, Np);		//if f1 P0p is positive then P0p is anticlockwise of V1, otherwise is clockwise

	subtract3fv(P0p, P2, tmp0);
	cross3fv(V2, tmp0, tmp1);
	f2 = dot3fv(tmp1, Np);		//if f2 P0p is positive then P0p is anticlockwise of V2, otherwise is clockwise

	subtract3fv(P0p, P3, tmp0);
	cross3fv(V3, tmp0, tmp1);
	f3 = dot3fv(tmp1, Np);		//if f3 P0p is positive then P0p is anticlockwise of V3, otherwise is clockwise

	//there is no combination of (+, +, +) and (-, -, -)
	//determine P0p locating in which region
	if (f1 >= 0 && f2 <= 0) {		//region V1 V2
		//determine P0p is outside or inside the triangle
		subtract3fv(P1, P0p, tmp0);
		subtract3fv(P2, P0p, tmp1);
		cross3fv(tmp0, tmp1, tmp2);
		if (dot3fv(tmp2, Np) < 0) {
			//determine P0p is close to vertex or edge
			subtract3fv(P2, P0p, tmp0);
			subtract3fv(P1, P0p, tmp1);
			cross3fv(tmp0, tmp1, tmp2);
			subtract3fv(P2, P1, tmp1);
			cross3fv(tmp2, tmp1, R);
			subtract3fv(P1, P0p, tmp0);
			cosGamma = dot3fv(tmp0, R) / (length3fv(tmp0) * length3fv(R));
			distp = length3fv(tmp0) * cosGamma;
			distp = fabs(distp);
			tmp0[0] = tmp0[1] = tmp0[2] = distp;
			normalize3fv(R);
			mult3fv(tmp0, R, P0p_P0pp);
			add3fv(P0p, P0p_P0pp, P0pp);
			subtract3fv(P0pp, P1, tmp0);
			subtract3fv(P2, P1, tmp1);
			t = length3fv(tmp0) / length3fv(tmp1) * (dot3fv(tmp0, tmp1) / fabs(dot3fv(tmp0, tmp1)));
			if (t >= 0 && t <= 1) {			//edge P1 P2
				projPnt[0] = P0pp[0]; projPnt[1] = P0pp[1]; projPnt[2] = P0pp[2];
				return sign * sqrt(dist * dist + distp * distp);
			}
			else if (t < 0) {				//vertex P1
				projPnt[0] = P1[0]; projPnt[1] = P1[1]; projPnt[2] = P1[2];
				subtract3fv(P0, P1, tmp0);
				return sign * length3fv(tmp0);
			}
			else {							//vertex P2
				projPnt[0] = P2[0]; projPnt[1] = P2[1]; projPnt[2] = P2[2];
				subtract3fv(P0, P2, tmp0);
				return sign * length3fv(tmp0);
			}
		}
		else {
			projPnt[0] = P0p[0]; projPnt[1] = P0p[1]; projPnt[2] = P0p[2];
			return sign * dist;
		}
	}
	else if (f2 >= 0 && f3 <= 0) {	//region V2 V3
		//determine P0p is outside or inside the triangle
		subtract3fv(P2, P0p, tmp0);
		subtract3fv(P3, P0p, tmp1);
		cross3fv(tmp0, tmp1, tmp2);
		if (dot3fv(tmp2, Np) < 0) {
			//determine P0p is close to vertex or edge
			subtract3fv(P3, P0p, tmp0);
			subtract3fv(P2, P0p, tmp1);
			cross3fv(tmp0, tmp1, tmp2);
			subtract3fv(P3, P2, tmp1);
			cross3fv(tmp2, tmp1, R);
			subtract3fv(P2, P0p, tmp0);
			cosGamma = dot3fv(tmp0, R) / (length3fv(tmp0) * length3fv(R));
			distp = length3fv(tmp0) * cosGamma;
			distp = fabs(distp);
			tmp0[0] = tmp0[1] = tmp0[2] = distp;
			normalize3fv(R);
			mult3fv(tmp0, R, P0p_P0pp);
			add3fv(P0p, P0p_P0pp, P0pp);
			subtract3fv(P0pp, P2, tmp0);
			subtract3fv(P3, P2, tmp1);
			t = length3fv(tmp0) / length3fv(tmp1) * (dot3fv(tmp0, tmp1) / fabs(dot3fv(tmp0, tmp1)));
			if (t >= 0 && t <= 1) {		//edge P2 P3
				projPnt[0] = P0pp[0]; projPnt[1] = P0pp[1]; projPnt[2] = P0pp[2];
				return sign * sqrt(dist * dist + distp * distp);
			}
			else if (t < 0) {			//vertex P2
				projPnt[0] = P2[0]; projPnt[1] = P2[1]; projPnt[2] = P2[2];
				subtract3fv(P0, P2, tmp0);
				return sign * length3fv(tmp0);
			}
			else {						//vertex P3
				projPnt[0] = P3[0]; projPnt[1] = P3[1]; projPnt[2] = P3[2];
				subtract3fv(P0, P3, tmp0);
				return sign * length3fv(tmp0);
			}
		}
		else {
			projPnt[0] = P0p[0]; projPnt[1] = P0p[1]; projPnt[2] = P0p[2];
			return sign * dist;
		}
	}
	else {						//region V3 V1
		//determine P0p is outside or inside the triangle
		subtract3fv(P3, P0p, tmp0);
		subtract3fv(P1, P0p, tmp1);
		cross3fv(tmp0, tmp1, tmp2);
		if (dot3fv(tmp2, Np) < 0) {
			//determine P0p is close to vertex or edge
			subtract3fv(P1, P0p, tmp0);
			subtract3fv(P3, P0p, tmp1);
			cross3fv(tmp0, tmp1, tmp2);
			subtract3fv(P1, P3, tmp1);
			cross3fv(tmp2, tmp1, R);
			subtract3fv(P3, P0p, tmp0);
			cosGamma = dot3fv(tmp0, R) / (length3fv(tmp0) * length3fv(R));
			distp = length3fv(tmp0) * cosGamma;
			distp = fabs(distp);
			tmp0[0] = tmp0[1] = tmp0[2] = distp;
			normalize3fv(R);
			mult3fv(tmp0, R, P0p_P0pp);
			add3fv(P0p, P0p_P0pp, P0pp);
			subtract3fv(P0pp, P3, tmp0);
			subtract3fv(P1, P3, tmp1);
			t = length3fv(tmp0) / length3fv(tmp1) * (dot3fv(tmp0, tmp1) / fabs(dot3fv(tmp0, tmp1)));
			if (t >= 0 && t <= 1) {		//edge P3 P1
				projPnt[0] = P0pp[0]; projPnt[1] = P0pp[1]; projPnt[2] = P0pp[2];
				return sign * sqrt(dist * dist + distp * distp);
			}
			else if (t < 0) {			//vertex P3
				projPnt[0] = P3[0]; projPnt[1] = P3[1]; projPnt[2] = P3[2];
				subtract3fv(P0, P3, tmp0);
				return sign * length3fv(tmp0);
			}
			else {						//vertex P1
				projPnt[0] = P1[0]; projPnt[1] = P1[1]; projPnt[2] = P1[2];
				subtract3fv(P0, P1, tmp0);
				return sign * length3fv(tmp0);
			}
		}
		else {
			projPnt[0] = P0p[0]; projPnt[1] = P0p[1]; projPnt[2] = P0p[2];
			return sign * dist;
		}
	}
}