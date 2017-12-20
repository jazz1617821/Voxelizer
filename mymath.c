#include "mymath.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.1415926

static bool gs_isFirstRandom = true;

int precisionCompare(float a, float b, int precision)
{
	int i, ret, ia, ib;

	ia = (int)roundf(a * precision);
	ib = (int)roundf(b * precision);
	if (ia < ib)
		return -1;
	if (ia == ib)
		return 0;
	if (ia > ib)
		return 1;
}

int precisionCompare2fv(float a[2], float b[2], int precision)
{
	int i, ret, ia[2], ib[2];

	ia[0] = (int)roundf(a[0] * precision);
	ia[1] = (int)roundf(a[1] * precision);
	ib[0] = (int)roundf(b[0] * precision);
	ib[1] = (int)roundf(b[1] * precision);
	if (ia[0] < ib[0] && ia[1] < ib[1])
		return -1;
	if (ia[0] == ib[0] && ia[1] == ib[1])
		return 0;
	if (ia[0] > ib[0] && ia[1] > ib[1])
		return 1;
}

int precisionCompare3fv(float a[3], float b[3], int precision)
{
	int i, ret, ia[3], ib[3];

	ia[0] = (int)roundf(a[0] * precision);
	ia[1] = (int)roundf(a[1] * precision);
	ia[2] = (int)roundf(a[2] * precision);
	ib[0] = (int)roundf(b[0] * precision);
	ib[1] = (int)roundf(b[1] * precision);
	ib[2] = (int)roundf(b[2] * precision);
	if (ia[0] < ib[0] && ia[1] < ib[1] && ia[2] < ib[2])
		return -1;
	if (ia[0] == ib[0] && ia[1] == ib[1] && ia[2] == ib[2])
		return 0;
	if (ia[0] > ib[0] && ia[1] > ib[1] && ia[2] > ib[2])
		return 1;
}

int precisionCompare4fv(float a[4], float b[4], int precision)
{
	int i, ret, ia[4], ib[4];

	ia[0] = (int)roundf(a[0] * precision);
	ia[1] = (int)roundf(a[1] * precision);
	ia[2] = (int)roundf(a[2] * precision);
	ia[3] = (int)roundf(a[3] * precision);
	ib[0] = (int)roundf(b[0] * precision);
	ib[1] = (int)roundf(b[1] * precision);
	ib[2] = (int)roundf(b[2] * precision);
	ib[3] = (int)roundf(b[3] * precision);
	if (ia[0] < ib[0] && ia[1] < ib[1] && ia[2] < ib[2] && ia[3] < ib[3])
		return -1;
	if (ia[0] == ib[0] && ia[1] == ib[1] && ia[2] == ib[2] && ia[3] == ib[3])
		return 0;
	if (ia[0] > ib[0] && ia[1] > ib[1] && ia[2] > ib[2] && ia[3] > ib[3])
		return 1;
}

void precisionZero(float *a, int precision)
{
	if (precisionCompare(*a, 0, precision) == 0) {
		*a = 0.0;
	}
}

void precisionZero2fv(float a[2], int precision)
{
	int i;

	for (i = 0; i < 2; ++i)
	{
		if (precisionCompare(a[i], 0, precision) == 0) {
			a[i] = 0.0;
		}
	}
}

void precisionZero3fv(float a[3], int precision)
{
	int i;

	for (i = 0; i < 3; ++i)
	{
		if (precisionCompare(a[i], 0, precision) == 0) {
			a[i] = 0.0;
		}
	}
}

void precisionZero4fv(float a[4], int precision)
{
	int i;

	for (i = 0; i < 4; ++i)
	{
		if (precisionCompare(a[i], 0, precision) == 0) {
			a[i] = 0.0;
		}
	}
}

float radians(float deg)
{
	return deg * PI / 180.0;
}

float degrees(float rad)
{
	return rad / PI * 180.0;
}

void copy2fv(float src[2], float des[2])
{
	des[0] = src[0];
	des[1] = src[1];
}

void copy3fv(float src[3], float des[3])
{
	des[0] = src[0];
	des[1] = src[1];
	des[2] = src[2];
}

void copy4fv(float src[4], float des[4])
{
	des[0] = src[0];
	des[1] = src[1];
	des[2] = src[2];
	des[3] = src[3];
}

void copyMat4fv(float src[16], float des[16])
{
	int i;

	for (i = 0; i < 16; ++i)
	{
		des[i] = src[i];
	}
}

void negate2fv(const float a[2], float v[2])
{
	v[0] = -a[0];
	v[1] = -a[1];
}

void negate3fv(const float a[3], float v[3])
{
	v[0] = -a[0];
	v[1] = -a[1];
	v[2] = -a[2];
}

void negate4fv(const float a[4], float v[4])
{
	v[0] = -a[0];
	v[1] = -a[1];
	v[2] = -a[2];
	v[3] = -a[3];
}

void identifyMat2fv(float mat[4])
{
	mat[0] = 1;
	mat[1] = 0;
	mat[2] = 0;
	mat[3] = 1;
}

void identifyMat3fv(float mat[9])
{
	int i;

	for (i = 0; i < 9; ++i) {
		mat[i] = 0;
	}

	mat[0] = mat[4] = mat[7] = 1;
}

void identifyMat4fv(float mat[16])
{
	int i;

	for (i = 0; i < 16; ++i)
		mat[i] = 0;

	mat[0] = mat[5] = mat[10] = mat[15] = 1;
}

float length2fv(float v[2])
{
	return sqrt(v[0] * v[0] + v[1] * v[1]);
}

float length3fv(float v[3])
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void cross3fv(const float a[3], const float b[3], float v[3])
{
	v[0] = a[1] * b[2] - a[2] * b[1];
	v[1] = a[2] * b[0] - a[0] * b[2];
	v[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize3fv(float v[3])
{
	int i;
	float dist = length3fv(v);

	for (i = 0; i < 3; ++i) {
		v[i] /= dist;
	}
}

void normalize4fv(float v[4])
{
	int i;
	float dist;
	float a[3];

	for (i = 0; i < 3; ++i) {
		a[i] = v[i];
	}

	dist = length3fv(a);

	for (i = 0; i < 3; ++i) {
		v[i] /= dist;
	}
}

void add2fv(const float a[2], const float b[2], float v[2])
{
	int i;

	for (i = 0; i < 2; ++i)
		v[i] = a[i] + b[i];
}

void add3fv(const float a[3], const float b[3], float v[3])
{
	int i;

	for (i = 0; i < 3; ++i)
		v[i] = a[i] + b[i];
}

void add4fv(const float a[4], const float b[4], float v[4])
{
	int i;

	for (i = 0; i < 4; ++i)
		v[i] = a[i] + b[i];
}

void subtract2fv(const float a[2], const float b[2], float v[2])
{
	int i;

	for (i = 0; i < 2; ++i)
		v[i] = a[i] - b[i];
}

void subtract3fv(const float a[3], const float b[3], float v[3])
{
	int i;

	for (i = 0; i < 3; ++i)
		v[i] = a[i] - b[i];
}

void subtract4fv(const float a[4], const float b[4], float v[4])
{
	int i;

	for (i = 0; i < 4; ++i)
		v[i] = a[i] - b[i];
}

void mult2fv(const float a[2], const float b[2], float v[2])
{
	v[0] = a[0] * b[0];
	v[1] = a[1] * b[1];
}

void mult3fv(const float a[3], const float b[3], float v[3])
{
	v[0] = a[0] * b[0];
	v[1] = a[1] * b[1];
	v[2] = a[2] * b[2];
}

void mult4fv(const float a[4], const float b[4], float v[4])
{
	v[0] = a[0] * b[0];
	v[1] = a[1] * b[1];
	v[2] = a[2] * b[2];
	v[3] = a[3] * b[3];
}

void mult2x1fv(const float a[2], const float b, float v[2])
{
	v[0] = a[0] * b;
	v[1] = a[1] * b;
}

void mult3x1fv(const float a[3], const float b, float v[3])
{
	v[0] = a[0] * b;
	v[1] = a[1] * b;
	v[2] = a[2] * b;
}

void mult4x1fv(const float a[4], const float b, float v[4])
{
	v[0] = a[0] * b;
	v[1] = a[1] * b;
	v[2] = a[2] * b;
	v[3] = a[3] * b;
}

void multMat2fv(const float a[4], const float b[4], float mat[4])
{
	int i;
	float m[4];

	m[0] = a[0] * b[0] + a[1] * b[2];
	m[1] = a[0] * b[1] + a[1] * b[3];
	m[2] = a[2] * b[0] + a[3] * b[2];
	m[3] = a[2] * b[1] + a[3] * b[3];

	for (i = 0; i < 4; ++i) {
		mat[i] = m[i];
	}
}

void multMat3fv(const float a[9], const float b[9], float mat[9])
{
	int i, j, k;
	float m[9];

	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j) {
			m[i * 3 + j] = 0;
			for (k = 0; k < 3; ++k) {
				m[i * 3 + j] += a[i * 3 + k] * b[k * 3 + j];
			}
		}
	}

	for (i = 0; i < 9; ++i) {
		mat[i] = m[i];
	}
}

void multMat4fv(const float a[16], const float b[16], float mat[16])
{
	int i, j, k;
	float m[16];

	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			m[i * 4 + j] = 0;
			for (k = 0; k < 4; ++k) {
				m[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
			}
		}
	}

	for (i = 0; i < 16; ++i) {
		mat[i] = m[i];
	}
}

void multMat4x4fv(const float a[16], const float b[4], float mat[4])
{
	int i, j;
	float tmp[4], m[4];

	tmp[0] = b[0];
	tmp[1] = b[1];
	tmp[2] = b[2];
	tmp[3] = b[3];
	for (i = 0; i < 4; ++i) {
		m[i] = 0;
		for (j = 0; j < 4; ++j) {
			m[i] += a[i * 4 + j] * tmp[j];
		}
	}

	for (i = 0; i < 4; ++i) 
	{
		mat[i] = m[i];
	}
}

void multMat4x3fv(const float a[16], const float b[3], float mat[3])
{
	int i, j;
	float tmp[4], m[4];


	tmp[0] = b[0];
	tmp[1] = b[1];
	tmp[2] = b[2];
	tmp[3] = 1.0;
	for (i = 0; i < 4; ++i) 
	{
		m[i] = 0;
		for (j = 0; j < 4; ++j) 
		{
			m[i] += a[i * 4 + j] * tmp[j];
		}
	}

	for (i = 0; i < 3; ++i) 
	{
		mat[i] = m[i];
	}
}

void div2fv(const float a[2], const float b[2], float v[2])
{
	v[0] = a[0] / b[0];
	v[1] = a[1] / b[1];
}

void div3fv(const float a[3], const float b[3], float v[3])
{
	v[0] = a[0] / b[0];
	v[1] = a[1] / b[1];
	v[2] = a[2] / b[2];
}

void div4fv(const float a[4], const float b[4], float v[4])
{
	v[0] = a[0] / b[0];
	v[1] = a[1] / b[1];
	v[2] = a[2] / b[2];
	v[3] = a[3] / b[3];
}

void translate(float x, float y, float z, float mat[16])
{
	identifyMat4fv(mat);
	mat[3] = x;
	mat[7] = y;
	mat[11] = z;
}

void rotate(const float angle, const float axis[3], float mat[16])
{
	int i;
	float v[3] = { axis[0], axis[1], axis[2] };

	normalize3fv(v);

	float x = v[0];
	float y = v[1];
	float z = v[2];

	float c = cos(radians(angle));
	float omc = 1.0 - c;
	float s = sin(radians(angle));

	float m[16] = { x * x * omc + c, x * x * omc - z * s, x * z * omc + y * s, 0.0,
		x * y * omc + z * s, y * y * omc + c, y * z * omc - x * s, 0.0,
		x * z * omc - y * s, y * z * omc + x * s, z * z * omc + c, 0.0,
		0.0, 0.0, 0.0, 0.0 };

	for (i = 0; i < 16; ++i)
		mat[i] = m[i];
}

void rotateX(float theta, float mat[16])
{
	int i;
	float c = cos(radians(theta));
	float s = sin(radians(theta));

	float m[16] = { 1.0, 0.0, 0.0, 0.0,
		0.0, c, s, 0.0,
		0.0, -s, c, 0.0,
		0.0, 0.0, 0.0, 1.0 };

	for (i = 0; i < 16; ++i)
		mat[i] = m[i];
}

void rotateY(float theta, float mat[16])
{
	int i;
	float c = cos(radians(theta));
	float s = sin(radians(theta));

	float m[16] = { c, 0.0, -s, 0.0,
		0.0, 1.0, 0.0, 0.0,
		s, 0.0, c, 0.0,
		0.0, 0.0, 0.0, 1.0 };

	for (i = 0; i < 16; ++i)
		mat[i] = m[i];
}

void rotateZ(float theta, float mat[16])
{
	int i;
	float c = cos(radians(theta));
	float s = sin(radians(theta));

	float m[16] = { c, -s, 0.0, 0.0,
		s, c, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 };

	for (i = 0; i < 16; ++i)
		mat[i] = m[i];
}

void scale(float x, float y, float z, float mat[16])
{
	identifyMat4fv(mat);
	mat[0] = x;
	mat[5] = y;
	mat[10] = z;
}

void transposeMat2fv(const float m[4], float mat[4])
{
	int i, j;

	for (i = 0; i < 2; ++i) {
		for (j = 0; j < 2; ++j) {
			mat[i * 2 + j] = m[j * 2 + i];
		}
	}
}

void transposeMat3fv(const float m[9], float mat[9])
{
	int i, j;

	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j) {
			mat[i * 3 + j] = m[j * 3 + i];
		}
	}
}

void transposeMat4fv(const float m[16], float mat[16])
{
	int i, j;

	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			mat[i * 4 + j] = m[j * 4 + i];
		}
	}
}

void transposeMat4(float mat[16])
{
	int i, j;
	float tmp;

	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			if (j <= i) continue;
			tmp = mat[i * 4 + j];
			mat[i * 4 + j] = mat[j * 4 + i];
			mat[j * 4 + i] = tmp;
		}
	}
}

float dot2fv(const float a[2], const float b[2])
{
	int i;
	float sum;

	for (i = 0, sum = 0; i < 2; ++i) {
		sum += a[i] * b[i];
	}
	return sum;
}

float dot3fv(const float a[3], const float b[3])
{
	int i;
	float sum;

	for (i = 0, sum = 0; i < 3; ++i) {
		sum += a[i] * b[i];
	}
	return sum;
}

float dot4fv(const float a[4], const float b[4])
{
	int i;
	float sum;

	for (i = 0, sum = 0; i < 4; ++i) {
		sum += a[i] * b[i];
	}
	return sum;
}

float det2fv(const float m[4])
{
	return m[0] * m[3] - m[1] * m[2];
}

float det3fv(const float m[3])
{
	return	m[0] * m[4] * m[8] +
		m[1] * m[5] * m[6] +
		m[2] * m[3] * m[7] -
		m[2] * m[4] * m[6] -
		m[1] * m[3] * m[8] -
		m[0] * m[5] * m[7];
}

float det4fv(const float m[16])
{
	float m0[9] = { m[5], m[6], m[7],
		m[9], m[10], m[11],
		m[13], m[14], m[15] };

	float m1[9] = { m[4], m[6], m[7],
		m[8], m[10], m[11],
		m[12], m[14], m[15] };

	float m2[9] = { m[4], m[5], m[7],
		m[8], m[9], m[11],
		m[12], m[13], m[15] };

	float m3[9] = { m[4], m[5], m[6],
		m[8], m[9], m[10],
		m[12], m[13], m[14] };

	return m[0] * det3fv(m0) - m[1] * det3fv(m1) + m[2] * det3fv(m2) - m[3] * det3fv(m3);
}

void inverse2fv(const float m[4], float mat[4])
{
	float d = det2fv(m);

	mat[0] = m[3] / d;
	mat[1] = -m[1] / d;
	mat[2] = -m[2] / d;
	mat[3] = m[0] / d;
}

void inverse3fv(const float m[9], float mat[9])
{
	float d = det3fv(m);

	float a00[4] = { m[4], m[5],
		m[7], m[8] };

	float a01[4] = { m[3], m[5],
		m[6], m[8] };

	float a02[4] = { m[3], m[4],
		m[6], m[7] };

	float a10[4] = { m[1], m[2],
		m[7], m[8] };

	float a11[4] = { m[0], m[2],
		m[6], m[8] };

	float a12[4] = { m[0], m[1],
		m[6], m[7] };

	float a20[4] = { m[1], m[2],
		m[4], m[5] };

	float a21[4] = { m[0], m[2],
		m[3], m[5] };

	float a22[4] = { m[0], m[1],
		m[3], m[4] };

	mat[0] = det2fv(a00) / d;
	mat[1] = -det2fv(a01) / d;
	mat[2] = det2fv(a20) / d;
	mat[3] = -det2fv(a01) / d;
	mat[4] = det2fv(a11) / d;
	mat[5] = -det2fv(a21) / d;
	mat[6] = det2fv(a02) / d;
	mat[7] = -det2fv(a12) / d;
	mat[8] = det2fv(a22) / d;
}

void inverse4fv(const float m[16], float mat[16])
{
	float d = det4fv(m);

	float a00[9] = { m[5], m[6], m[7],
		m[9], m[10], m[11],
		m[13], m[14], m[15] };

	float a01[9] = { m[4], m[6], m[7],
		m[8], m[10], m[11],
		m[12], m[14], m[15] };

	float a02[9] = { m[4], m[5], m[7],
		m[8], m[9], m[11],
		m[12], m[14], m[15] };

	float a03[9] = { m[4], m[5], m[6],
		m[8], m[9], m[10],
		m[12], m[13], m[14] };

	float a10[9] = { m[1], m[2], m[3],
		m[9], m[10], m[11],
		m[13], m[14], m[15] };

	float a11[9] = { m[0], m[2], m[3],
		m[8], m[10], m[11],
		m[12], m[14], m[15] };

	float a12[9] = { m[0], m[1], m[3],
		m[8], m[9], m[11],
		m[12], m[13], m[15] };

	float a13[9] = { m[0], m[1], m[2],
		m[8], m[9], m[10],
		m[12], m[13], m[14] };

	float a20[9] = { m[1], m[2], m[3],
		m[5], m[6], m[7],
		m[13], m[14], m[15] };

	float a21[9] = { m[0], m[2], m[3],
		m[4], m[6], m[7],
		m[12], m[14], m[15] };

	float a22[9] = { m[0], m[1], m[3],
		m[4], m[5], m[7],
		m[12], m[13], m[15] };

	float a23[9] = { m[0], m[1], m[2],
		m[4], m[5], m[6],
		m[12], m[13], m[14] };

	float a30[9] = { m[1], m[2], m[3],
		m[5], m[6], m[7],
		m[9], m[10], m[11] };

	float a31[9] = { m[0], m[2], m[3],
		m[4], m[6], m[7],
		m[8], m[10], m[11] };

	float a32[9] = { m[0], m[1], m[3],
		m[4], m[5], m[7],
		m[8], m[9], m[11] };

	float a33[9] = { m[0], m[1], m[2],
		m[4], m[5], m[6],
		m[8], m[9], m[10] };

	mat[0] = det3fv(a00) / d;
	mat[1] = -det3fv(a10) / d;
	mat[2] = det3fv(a20) / d;
	mat[3] = -det3fv(a30) / d;
	mat[4] = -det3fv(a01) / d;
	mat[5] = det3fv(a11) / d;
	mat[6] = -det3fv(a21) / d;
	mat[7] = det3fv(a31) / d;
	mat[8] = det3fv(a02) / d;
	mat[9] = -det3fv(a12) / d;
	mat[10] = det3fv(a22) / d;
	mat[11] = -det3fv(a32) / d;
	mat[12] = -det3fv(a03) / d;
	mat[13] = det3fv(a13) / d;
	mat[14] = -det3fv(a23) / d;
	mat[15] = det3fv(a33) / d;
}

bool equal2fv(const float a[2], const float b[2], const float precision)
{
	int i;

	for (i = 0; i < 2; ++i)
	{
		if (fabs(a[i] - b[i]) > precision) {
			return false;
		}
	}

	return true;
}

bool equal3fv(const float a[3], const float b[3], const float precision)
{
	int i;

	for (i = 0; i < 3; ++i)
	{
		if (fabs(a[i] - b[i]) > precision) {
			return false;
		}
	}

	return true;
}

bool equal4fv(const float a[4], const float b[4], const float precision)
{
	int i;

	for (i = 0; i < 4; ++i)
	{
		if (fabs(a[i] - b[i]) > precision) {
			return false;
		}
	}

	return true;
}

void normalM(const float modelview[16], float mat[16])
{
	float m[16];

	inverse4fv(modelview, m);
	transposeMat4fv(m, mat);
}

void lookAt(const float eye[3], const float lok[3], const float vup[3], float mat[16])
{
	int i;
	float w[3], nw[3], upN[3], v[3], u[3];

	for (i = 0; i < 3; ++i)	{
		w[i] = eye[i] - lok[i];
		upN[i] = vup[i];
	}
	normalize3fv(w);
	normalize3fv(upN);
	negate3fv(w, nw);
	cross3fv(nw, upN, u);
	normalize3fv(u);
	cross3fv(w, u, v);
	normalize3fv(v);

	float m[16] = { u[0], u[1], u[2], 0,
		v[0], v[1], v[2], 0,
		w[0], w[1], w[2], 0,
		0, 0, 0, 1 };

	m[3] = u[0] * -eye[0] + u[1] * -eye[1] + u[2] * -eye[2];
	m[7] = v[0] * -eye[0] + v[1] * -eye[1] + v[2] * -eye[2];
	m[11] = w[0] * -eye[0] + w[1] * -eye[1] + w[2] * -eye[2];

	for (i = 0; i < 16; ++i) {
		mat[i] = m[i];
	}
}

void ortho(const float left, const float right, const float bottom, const float top, const float nearP, const float farP, float mat[16])
{
	if (left == right) {
		fprintf(stderr, "left value equals right value");
	}
	if (top == bottom) {
		fprintf(stderr, "top value equals bottom value");
	}
	if (nearP == farP) {
		fprintf(stderr, "near value equals far far value");
	}

	int i;
	float w = right - left;
	float h = top - bottom;
	float d = farP - nearP;

	float m[16] = { 0 };

	m[0] = 2.0 / w;
	m[3] = -(left + right) / w;
	m[5] = 2.0 / h;
	m[7] = -(top + bottom) / h;
	m[10] = -2.0 / d;
	m[11] = -(nearP + farP) / d;
	m[15] = 1.0;

	for (i = 0; i < 16; ++i) {
		mat[i] = m[i];
	}
}

void perspective(const float fovy, const float aspect, const float nearP, const float farP, float mat[16])
{
	int i;
	float f = 1.0 / tan(radians(fovy) / 2);
	float d = farP - nearP;

	float m[16] = { 0 };

	m[0] = f / aspect;
	m[5] = f;
	m[10] = -(nearP + farP) / d;
	m[11] = -2 * nearP * farP / d;
	m[14] = -1;
	m[15] = 0.0;

	for (i = 0; i < 16; ++i) {
		mat[i] = m[i];
	}
}

void compGradient(float values[6], float dx, float dy, float dz, float vec[3])
{
	vec[0] = (values[1] - values[0]) / dx;
	vec[1] = (values[3] - values[2]) / dy;
	vec[2] = (values[5] - values[4]) / dz;
	normalize3fv(vec);
}

void compPlane(float normal[3], float point[3], float param[4])
{
	param[3] = -(normal[0] * point[0] + normal[1] * point[1] + normal[2] * point[2]);
	param[2] = normal[2];
	param[1] = normal[1];
	param[0] = normal[0];
}

float compDistFromPointToPlane(float param[4], float point[3])
{
	return fabs((param[0] * point[0] + param[1] * point[1] + param[2] * point[2] + param[3]) / 
		   (param[0] * param[0] + param[1] * param[1] + param[2] * param[2]));
}

void compPntOnPlane(float param[4], float o[3], float dir[3], float point[3])
{
	float a, b, t;
	
	a = -(param[0] * o[0] + param[1] * o[1] + param[2] * o[2] + param[3]);
	b = (param[0] * dir[0] + param[1] * dir[1] + param[2] * dir[2]);

	if (precisionCompare(b, 0, PRECISION5) == 0)
		t = 0;
	else
		t = a / b;

	point[0] = o[0] + dir[0] * t;
	point[1] = o[1] + dir[1] * t;
	point[2] = o[2] + dir[2] * t;
}

void compNormal(float vertices[9], float normal[3])
{
	float P1[3] = { vertices[0], vertices[1], vertices[2] };
	float P2[3] = { vertices[3], vertices[4], vertices[5] };
	float P3[3] = { vertices[6], vertices[7], vertices[8] };
	float tmp0[3], tmp1[3];

	subtract3fv(P2, P1, tmp0);
	subtract3fv(P3, P1, tmp1);
	cross3fv(tmp0, tmp1, normal);
	normalize3fv(normal);
}

int compPointPlane(float param[4], float point[3])
{
	float value = param[0] * point[0] + param[1] * point[1] + param[2] * point[2] + param[3];
	if (value > 0) {
		return 1;
	}
	else if (value == 0) {
		return 0;
	}
	else {
		return -1;
	}
}

void zupMat(float mat[16])
{
	mat[0] = 1; mat[1] = 0; mat[2] = 0; mat[3] = 0;
	mat[4] = 0; mat[5] = 1; mat[6] = 0; mat[7] = 0;
	mat[8] = 0; mat[9] = 0; mat[10] = -1; mat[11] = 0;
	mat[12] = 1; mat[13] = 0; mat[14] = 0; mat[15] = 1;
}


float randomF(float value)
{
	if (gs_isFirstRandom) {
		srand((unsigned int)time(NULL));
		gs_isFirstRandom = false;
	}

	float result = ((float)rand() / (float)(RAND_MAX)) * value;

	return result;
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}