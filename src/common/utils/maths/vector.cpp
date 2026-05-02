#include "pch.h"
#include "vector.h"

float Vector3::magnitude(bool _squared) {
	return _squared ? (x * x + y * y + z * z)
		: sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::normalized() {
	float mag = magnitude();
	if (mag == 0.0f) {
		return Vector3{ 0.0f, 0.0f, 0.0f };
	}
	return Vector3{ x / mag, y / mag, z / mag };
}

void Vector3::normalize() {
	float mag = magnitude();
	if (mag == 0.0f) {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		return;
	}
	x /= mag;
	y /= mag;
	z /= mag;
}