#pragma once
#include "glm/glm.hpp"

struct Vector2 {
	float x, y;
	Vector2() : x(0), y(0) {}                 // default ctor
	Vector2(float x1, float y1) : x(x1), y(y1) {}

	float Magnitude(Vector2 p1) {
		float x = p1.x - this->x;
		float y = p1.y - this->y;

		return sqrtf(x * x + y * y);
	}
	float MagnitudeSquaredFast(Vector2 p1) {
		float x = p1.x - this->x;
		float y = p1.y - this->y;

		return (x * x + y * y);
	}

	Vector2& operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
		return *this; // return reference to self
	}

	Vector2 operator +(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 operator *(const Vector2& other) const {
		return Vector2(x * other.x, y * other.y);
	}

	Vector2 operator -(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 operator*(float scalar) const {
		return Vector2(x * scalar, y * scalar);
	}
	Vector2 operator +(float scalar) const {
		return Vector2(x + scalar, y + scalar);
	}
	Vector2 operator -(float scalar) const {
		return Vector2(x - scalar, y - scalar);
	}
	Vector2& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
	operator glm::vec2() const {
		return glm::vec2(x, y);
	}
	Vector2(const glm::vec2& v) : x(v.x), y(v.y) {}
};



struct Vector3 {
	float x, y,z;
	Vector3() : x(0), y(0), z(0) {}                 // default ctor
	Vector3(float x1, float y1,float z1) : x(x1), y(y1),z(z1) {}
};

struct Vector4 {
	float x, y, z, w;
	Vector4() : x(0), y(0), z(0), w(0) {}                 // default ctor
	Vector4(float x1, float y1, float z1, float w1) : x(x1), y(y1), z(z1), w(w1) {}
	static Vector4 RBGAtoV4(float r, float g, float b,float a) {
		return Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}
};
