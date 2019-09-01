#pragma once
#ifndef VEC2_H
#define VEC2_H

#include<math.h>

class vec2 {
public:
	vec2() {}
	vec2(float x, float y) { e[0] = x;	 e[1] = y; }

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }

	inline vec2 operator -() const { return vec2(-e[0], -e[1]); }
	inline float operator[] (int i) const { return e[i]; }
	inline float& operator[] (int i) { return e[i]; }

	inline vec2& operator+=(const vec2 &v2);
	inline vec2& operator-=(const vec2 &v2);
	inline vec2& operator*=(const vec2 &v2);
	inline vec2& operator/=(const vec2 &v2);
	inline vec2& operator*=(const float &t);
	inline vec2& operator/=(const float &t);
	inline vec2 operator*(const float &t);

	inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1]); }; //length of vector
	inline float squared_length() const { return e[0] * e[0] + e[1] * e[1]; }; //squared length of vector
	inline void make_unit_vector(); //normalize vector

	float e[2]; //x,y
};

inline vec2 vec2::operator*(const float &t) {
	return vec2(t*e[0], t*e[1]);
}

inline vec2& vec2::operator+=(const vec2 &v2) {
	e[0] += v2.e[0];
	e[1] += v2.e[1];
	return *this;
}

inline vec2& vec2::operator-= (const vec2 &v2) {
	e[0] -= v2.e[0];
	e[1] -= v2.e[1];
	return *this;
}

inline vec2& vec2::operator*= (const vec2 &v2) {
	e[0] *= v2.e[0];
	e[1] *= v2.e[1];
	return *this;
}

inline vec2& vec2::operator/= (const vec2 &v2) {
	e[0] += v2.e[0];
	e[1] += v2.e[1];
	return *this;
}

inline vec2& vec2::operator*= (const float &t) {
	e[0] *= t;
	e[1] *= t;
	return *this;
}

inline vec2& vec2::operator/= (const float &t) {
	e[0] /= t;
	e[1] /= t;
	return *this;
}

inline vec2 operator+(const vec2 &v1, const vec2 &v2) {
	return vec2(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1]);
}

inline vec2 operator-(const vec2 &v1, const vec2 &v2) {
	return vec2(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1]);
}

inline vec2 operator*(const vec2 &v1, const float &t) {
	return vec2(v1.e[0] * t, v1.e[1] * t);
}

inline vec2 operator*(const float &t, const vec2 &v1) {
	return vec2(v1.e[0] * t, v1.e[1] * t);
}

inline vec2 operator/(const vec2 &v1, const float &t) {
	return vec2(v1.e[0] / t, v1.e[1] / t);
}


inline vec2 operator/(const float &t, const vec2 &v1) {
	return vec2(v1.e[0] / t, v1.e[1] / t);
}

inline void vec2::make_unit_vector() {
	float k = 1.0f / sqrt(e[0] * e[0] + e[1] * e[1]);
	e[0] *= k; e[1] *= k;
}

inline vec2 unit_vector(vec2 &v) {
	return v / v.length();
}

#endif // !VEC2_H
