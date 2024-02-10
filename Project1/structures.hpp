#pragma once
#include"includes.hpp"
#include"offsets.h"
enum class HitboxType {
	Head = 0,
	Neck = 1,
	UpperChest = 2,
	LowerChest = 3,
	Stomach = 4,
	Hip = 5
};
struct Color {
	float red;
	float green;
	float blue;

	bool operator==(const Color& other) const {
		return (red == other.red) && (green == other.green) && (blue == other.blue);
	}

	bool operator!=(const Color& other) const {
		return !(*this == other);
	}
};

struct GlowMode {
	std::byte bodyStyle, borderStyle, borderWidth, transparency;

	GlowMode() {}

	GlowMode(int bodyStyle_val, int borderStyle_val, int borderWidth_val, int transparency_val) :
		bodyStyle(static_cast<std::byte>(bodyStyle_val)),
		borderStyle(static_cast<std::byte>(borderStyle_val)),
		borderWidth(static_cast<std::byte>(borderWidth_val)),
		transparency(static_cast<std::byte>(transparency_val)) {}


	GlowMode(std::byte bodyStyle_val, std::byte borderStyle_val, std::byte borderWidth_val, std::byte transparency_val) :
		bodyStyle(bodyStyle_val),
		borderStyle(borderStyle_val),
		borderWidth(borderWidth_val),
		transparency(transparency_val) {}

	bool isZeroVector() const {
		return bodyStyle == std::byte(0)
			&& borderStyle == std::byte(0)
			&& borderWidth == std::byte(0)
			&& borderWidth == std::byte(0);
	}

	void print() const {
		std::cout
			<< "bodyStyle:" << static_cast<int>(bodyStyle)
			<< " borderStyle:" << static_cast<int>(borderStyle)
			<< " borderWidth:" << static_cast<int>(borderWidth)
			<< " transparency:" << static_cast<int>(transparency)
			<< "\n";
	}

	bool operator==(const GlowMode& other) const {
		return bodyStyle == other.bodyStyle
			&& borderStyle == other.borderStyle
			&& borderWidth == other.borderWidth
			&& transparency == other.transparency;
	}

	bool operator!=(const GlowMode& other) const {
		return !(*this == other);
	}
};
struct Vector3 {
	float x, y, z;
};

struct Matrix {
	float matrix[16];
};
struct Vec3 {
	float x, y, z;
	Vec3() { x = 0; y = 0; z = 0; }
	Vec3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	bool operator==(const float _t) {
		return x == _t && y == _t && z == _t;
	}
	bool operator==(Vec3 _t) {
		return x == _t.x && y == _t.y && z == _t.z;
	}
	bool operator!=(Vec3 _t) {
		return x != _t.x && y != _t.y && z != _t.z;
	}
	bool operator<(const float _t) {
		return x < _t && y < _t && z < _t;
	}
	bool operator>(const float _t) {
		return x > _t && y > _t && z > _t;
	}
	bool operator>=(const float _t) {
		return x >= _t && y >= _t && z >= _t;
	}
	bool operator<=(const float _t) {
		return x <= _t && y <= _t && z <= _t;
	}
	bool operator!=(const float _t) {
		return x != _t && y != _t && z != _t;
	}
};
struct Vec2 {
	float x, y;
	Vec2() { x = 0; y = 0; }
	Vec2(float _x, float _y) { x = _x; y = _y; }
	Vec2(Vec3 _t) { x = _t.x; y = _t.y; }
	bool operator==(const float _t) {
		return x == _t && y == _t;
	}
	bool operator<(const float _t) {
		return x < _t && y < _t;
	}
	bool operator>(const float _t) {
		return x > _t && y > _t;
	}
	bool operator>=(const float _t) {
		return x >= _t && y >= _t;
	}
	bool operator<=(const float _t) {
		return x <= _t && y <= _t;
	}
	bool operator!=(const float _t) {
		return x != _t && y != _t;
	}
};
struct QAngle {

	float x;
	float y;

	QAngle() : x(0), y(0) {}

	QAngle(float x, float y) : x(x), y(y) {}

	inline QAngle operator+(const QAngle& other) const {
		return QAngle(x + other.x, y + other.y);
	}

	inline QAngle operator-(const QAngle& other) const {
		return QAngle(x - other.x, y - other.y);
	}

	inline QAngle operator*(const float scalar) const {
		return QAngle(x * scalar, y * scalar);
	}

	inline QAngle operator/(const float scalar) const {
		return QAngle(x / scalar, y / scalar);
	}

	inline QAngle& operator+=(const QAngle& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	inline QAngle& operator-=(const QAngle& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline QAngle& operator*=(const float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	inline QAngle& operator/=(const float scalar) {
		x /= scalar;
		y /= scalar;
		return *this;
	}

	inline bool operator==(const QAngle& other) const
	{
		return x == other.x && y == other.y;
	}

	inline bool operator!=(const QAngle& other) const
	{
		return !(*this == other);
	}

	inline float dot(const QAngle& other) const {
		return x * other.x + y * other.y;
	}

	inline float length() const {
		return std::sqrt(x * x + y * y);
	}

	float distanceTo(const QAngle& other) const {
		return (other - *this).length();
	};

	inline QAngle& normalize() {
		float len = length();
		if (len > 0) {
			x /= len;
			y /= len;
		}
		return *this;
	}

	inline QAngle& clamp(float minVal, float maxVal) {
		x = std::clamp(x, minVal, maxVal);
		y = std::clamp(y, minVal, maxVal);

		return *this;
	}

	inline QAngle lerp(const QAngle& other, float t) const {
		return (*this) * (1.0f - t) + other * t;
	}

	inline QAngle& fixAngle() {
		if (!isValid()) {
			return *this;
		}

		while (y > 89.0f)
			y -= 180.f;

		while (y < -89.0f)
			y += 180.f;

		while (x > 180.f)
			x -= 360.f;

		while (x < -180.f)
			x += 360.f;

		return *this;
	}

	inline bool isValid() const {
		if (std::isnan(x) || std::isinf(x) || std::isnan(y) || std::isinf(y)) {
			return false;
		}

		return true;
	}

	inline static QAngle zero() {
		return QAngle(0, 0);
	}
};

struct Matrix3x4 {
public:
	float matrix[3][4];

	Vec3 GetPosition() const {
		return Vec3(matrix[0][3], matrix[1][3], matrix[2][3]);
	}

	Vec3 GetPosition2() const {
		return Vec3(matrix[0][3], matrix[1][3], matrix[2][3]);
	}
};