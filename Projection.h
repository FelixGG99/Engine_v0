#pragma once
#ifndef PROJH
#define PROJH
#include "vec.h"
#ifndef pi
#define pi 3.141592f
#endif // !pi

mat4x4 lookAt(const vec3 &camPos, const vec3 &target);
mat4x4 setProjMatrix(float w, float h, float n, float f, float fov);
float getScaleFromFOV(float fov, float n);

#endif // !PROJH

