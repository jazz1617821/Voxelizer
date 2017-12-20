#include "ray.h"
#include "mymath.h"

static void getNormalizedDeviecedCoordinates(const int mouseX, const int mouseY, const int width, const int height, float coord[2]);
static void getEyeCoords(const float clipCoords[4], const float proMat[16], float eyeCoords[4]);
static void getWorldCoords(const float eyeCoords[4], const float viewMat[16], float translates[3]);

void getRayDir(const int mouseX, const int mouseY,
			const int width, const int height,
			const float viewMat[16], const float proMat[16], float dir[3])
{
	int i;
	float normalizedCoords[2];
	float clipCoords[4];
	float eyeCoords[4];
	float worldRay[3];

	getNormalizedDeviecedCoordinates(mouseX, mouseY, width, height, normalizedCoords);
	clipCoords[0] = normalizedCoords[0];
	clipCoords[1] = normalizedCoords[1];
	clipCoords[2] = -1.0;
	clipCoords[3] = 1.0;
	getEyeCoords(clipCoords, proMat, eyeCoords);
	getWorldCoords(eyeCoords, viewMat, worldRay);

	for (i = 0; i < 3; ++i) {
		dir[i] = worldRay[i];
	}
}


// detect the projective point if inside the triangle or not
bool projPointTriangle(float projPnt[3], float vertex[9], float normal[3])
{
	float n[3], r0[3], r1[3], r2[3], r01[3], r12[3], r20[3];

	if (normal == NULL) {
		compNormal(vertex, n);
	}
	else {
		n[0] = normal[0];
		n[1] = normal[1];
		n[2] = normal[2];
	}

	// 3 Test
	subtract3fv(vertex, projPnt, r0);
	subtract3fv(vertex + 3, projPnt, r1);
	subtract3fv(vertex + 6, projPnt, r2);
	cross3fv(r0, r1, r01);
	cross3fv(r1, r2, r12);
	cross3fv(r2, r0, r20);
	if (dot3fv(r01, n) < 0)
		return false;

	if (dot3fv(r12, n) < 0)
		return false;
	
	if (dot3fv(r20, n) < 0)
		return false;

	return true;
}

// iterate the state of the ray
bool shootTriangle(float pos[3], float dir[3], float vertex[9], float proj[3])
{
	float normal[3], param[4], projPnt[3];
	bool isShot;

	compNormal(vertex, normal);
	compPlane(normal, vertex, param);
	compPntOnPlane(param, pos, dir, projPnt);
	isShot = projPointTriangle(projPnt, vertex, normal);

	if (proj != NULL) {
		proj[0] = projPnt[0];
		proj[1] = projPnt[1];
		proj[2] = projPnt[2];
	}

	return isShot;
}

static void getNormalizedDeviecedCoordinates(const int mouseX, const int mouseY, const int width, const int height, float coord[2])
{
	coord[0] = (2.0f * mouseX) / width - 1.0f;
	coord[1] = 1.0f - (2.0f * mouseY) / height;
}

static void getEyeCoords(const float clipCoords[4], const float proMat[16], float eyeCoords[4])
{
	int i;
	float invertMat[16];
	float eye[4];

	inverse4fv(proMat, invertMat);
	multMat4x4fv(invertMat, clipCoords, eye);
	eye[2] = -1.0;
	eye[3] = 0.0;

	for (i = 0; i < 4; ++i) {
		eyeCoords[i] = eye[i];
	}
}

static void getWorldCoords(const float eyeCoords[4], const float viewMat[16], float translates[3])
{
	int i;
	float invertMat[16];
	float world[4];

	inverse4fv(viewMat, invertMat);
	multMat4x4fv(invertMat, eyeCoords, world);
	normalize4fv(world);
	
	for (i = 0; i < 3; ++i) {
		translates[i] = world[i];
	}
}