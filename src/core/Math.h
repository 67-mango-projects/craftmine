#pragma once
#include "glm/glm.hpp"

struct Vector2 {
	float x, y;
	Vector2() : x(0), y(0) {}                 // default ctor
	Vector2(float x1, float y1) : x(x1), y(y1) {}

	operator glm::vec3() const {
		return glm::vec3(x, y, 0);
	}

	Vector2& operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
		return *this; // return reference to self
	}

	Vector2 operator +(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}
};

struct Vector3 {
	float x, y,z;
	Vector3() : x(0), y(0), z(0) {}                 // default ctor
	Vector3(float x1, float y1,float z1) : x(x1), y(y1),z(z1) {}
};

