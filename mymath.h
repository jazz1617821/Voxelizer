#ifndef _MYMATH_H
#define _MYMATH_H
#define _USE_MATH_DEFINES 
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define PI 3.1415926

#define PRECISION0 1
#define PRECISION1 10
#define PRECISION2 100
#define PRECISION3 1000
#define PRECISION4 10000
#define PRECISION5 100000
#define PRECISION6 1000000

int precisionCompare(float a, float b, int precision);
int precisionCompare2fv(float a[2], float b[2], int precision);
int precisionCompare3fv(float a[3], float b[3], int precision);
int precisionCompare4fv(float a[4], float b[4], int precision);
void precisionZero(float *a, int precision);
void precisionZero2fv(float a[2], int precision);
void precisionZero3fv(float a[3], int precision);
void precisionZero4fv(float a[4], int precision);
// convert degrees into radians
float radians(float deg);
// convert radians into degrees
float degrees(float rad);
void copy2fv(float src[2], float des[2]);
void copy3fv(float src[3], float des[3]);
void copy4fv(float src[4], float des[4]);
// copy values from src[16] to des[16]
void copyMat4fv(float src[16], float des[16]);
// negate a[2], return v[2]
void negate2fv(const float a[2], float v[2]);
// negate a[3], return v[3]
void negate3fv(const float a[3], float v[3]);
// negate a[4], return v[4]
void negate4fv(const float a[4], float v[4]);
// set mat[4] to be a identity matrix
void identifyMat2fv(float mat[4]);
// set mat[9] to be a identity matrix
void identifyMat3fv(float mat[9]);
// set mat[16] to be a identity matrix
void identifyMat4fv(float mat[16]);
// calculate vector length, v[2], and return length
float length2fv(float v[2]);
// calculate vector length, v[3], and return length
float length3fv(float v[3]);
void cross3fv(const float a[3], const float b[3], float v[3]);
void normalize3fv(float v[3]);
void normalize4fv(float v[4]);
void add2fv(const float a[2], const float b[2], float mat[2]);
void add3fv(const float a[3], const float b[3], float mat[3]);
void add4fv(const float a[4], const float b[4], float mat[4]);
void subtract2fv(const float a[2], const float b[2], float mat[2]);
void subtract3fv(const float a[3], const float b[3], float mat[3]);
void subtract4fv(const float a[4], const float b[4], float mat[4]);
void mult2fv(const float a[2], const float b[2], float v[2]);
void mult3fv(const float a[3], const float b[3], float v[3]);
void mult4fv(const float a[4], const float b[4], float v[4]);
void mult2x1fv(const float a[2], const float b, float v[2]);
void mult3x1fv(const float a[3], const float b, float v[3]);
void mult4x1fv(const float a[4], const float b, float v[4]);
void multMat2fv(const float a[4], const float b[4], float mat[4]);
void multMat3fv(const float a[9], const float b[9], float mat[9]);
void multMat4fv(const float a[16], const float b[16], float mat[16]);
void multMat4x4fv(const float a[16], const float b[4], float mat[4]);
void multMat4x3fv(const float a[16], const float b[3], float mat[3]);
void div2fv(const float a[2], const float b[2], float v[2]);
void div3fv(const float a[3], const float b[3], float v[3]);
void div4fv(const float a[4], const float b[4], float v[4]);
void translate(float x, float y, float z, float mat[16]);
void rotate(const float angle, const float axis[3], float mat[16]);
void rotateX(float theta, float mat[16]);
void rotateY(float theta, float mat[16]);
void rotateZ(float theta, float mat[16]);
void scale(float x, float y, float z, float mat[16]);
void transposeMat2fv(const float m[4], float mat[4]);
void transposeMat3fv(const float m[9], float mat[9]);
void transposeMat4fv(const float m[16], float mat[16]);
void transposeMat4(float mat[16]);
float dot2fv(const float a[2], const float b[2]);
float dot3fv(const float a[3], const float b[3]);
float dot4fv(const float a[4], const float b[4]);
float det2fv(const float m[4]);
float det3fv(const float m[3]);
float det4fv(const float m[16]);
void inverse2fv(const float m[4], float mat[4]);
void inverse3fv(const float m[9], float mat[9]);
void inverse4fv(const float m[16], float mat[16]);
bool equal2fv(const float a[2], const float b[2], const float precision);
bool equal3fv(const float a[3], const float b[3], const float precision);
bool equal4fv(const float a[4], const float b[4], const float precision);
void normalM(const float modelview[16], float mat[16]);
void lookAt(const float eye[3], const float lok[3], const float vup[3], float mat[16]);
void ortho(const float left, const float right, const float bottom, const float top, const float nearP, const float farP, float mat[16]);
void perspective(const float fovy, const float aspect, const float nearP, const float farP, float mat[16]);

void compGradient(float values[6], float dx, float dy, float dz, float vec[3]);
void compPlane(float normal[3], float point[3], float param[4]);
float compDistFromPointToPlane(float param[4], float point[3]);
int compPointPlane(float param[4], float point[3]);
void compPntOnPlane(float param[4], float o[3], float dir[3], float point[3]);
void compNormal(float vertices[9], float normal[3]);
void zupMat(float mat[16]);

// random a float number between [0.0 - value]
float randomF(float value);

float lerp(float a, float b, float f);

#endif