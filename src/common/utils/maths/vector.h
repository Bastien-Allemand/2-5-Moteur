#pragma once

struct Vector3 {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float magnitude(bool _squared = false);
	Vector3 normalized();

	void normalize();
	
	inline static const Vector3& zero() {
		return Vector3(0.0f, 0.0f, 0.0f);
	}
	
	inline static const Vector3& one() {
		return Vector3(1.0f, 1.0f, 1.0f);
	}

	inline Vector3 operator+(const Vector3& other) const {
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	inline Vector3 operator-(const Vector3& other) const {
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}

	inline Vector3 operator*(float scalar) const {
		return Vector3{ x * scalar, y * scalar, z * scalar };
	}

	inline Vector3 operator/(float scalar) const {
		return Vector3{ x / scalar, y / scalar, z / scalar };
	}

	inline Vector3& operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	inline Vector3& operator-=(const Vector3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	inline Vector3& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	inline Vector3& operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}
};