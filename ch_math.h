#pragma once

#include "ch_types.h"

// @TODO(CHall): Finish math lib
// Scalar Math
// Vector4
// Quaternion
// Plane
// Matrix4x4

namespace ch {

    f32 sqrt(f32 s);

    f32 cos(f32 s);

    f32 sin(f32 s);
    f32 asin(f32 s);

    f32 tan(f32 s);
    f32 atan(f32 s);
    f32 atan2(f32 x, f32 y);

	const f32 pi = 3.1415f;
	const f32 tau = (pi * 2.f);

	const f32 to_rad = pi / 180.f;
	const f32 to_deg = 180.f / pi;

	template <typename T>
	T clamp(T value, T min, T max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	struct Vector2 {
		union {
			struct { f32 x, y; };
			struct { f32 u, v; };
			struct { s32 ix, iy; };
            f32 xy[2];
            f32 uv[2];
		};

		Vector2() : x(0.f), y(0.f) {}
		Vector2(f32 xy) : x(xy), y(xy) {}
		Vector2(s32 ixy) : ix(ixy), iy(ixy) {}
        Vector2(f32 _x, f32 _y) : x(_x), y(_y) {}

        explicit operator bool() const { return x != 0.f || y != 0.f; }

        bool operator==(const Vector2& right) const {
            return x == right.x && y == right.y;
        }
        
        bool operator!=(const Vector2& right) const {
            return !(*this == right);
        }

        CH_FORCEINLINE Vector2 operator+(const Vector2& right) const {
            return Vector2(x + right.x, y + right.y);
        }

        CH_FORCEINLINE void operator+=(const Vector2& right) {
            *this = *this + right;
        }

        CH_FORCEINLINE Vector2 operator+(f32 s) const {
            return Vector2(x + s, y + s);
        }

        CH_FORCEINLINE void operator+=(f32 s) {
            *this = *this + s;
        }

        CH_FORCEINLINE Vector2 operator-(const Vector2& right) const {
            return Vector2(x - right.x, y - right.y);
        }

        CH_FORCEINLINE void operator-=(const Vector2& right) {
            *this = *this - right;
        }

        CH_FORCEINLINE Vector2 operator-(f32 s) const {
            return Vector2(x - s, y - s);
        }

        CH_FORCEINLINE void operator-=(f32 s) {
            *this = *this - s;
        }

        CH_FORCEINLINE Vector2 operator*(const Vector2& right) const {
            return Vector2(x * right.x, y * right.y);
        }

        CH_FORCEINLINE void operator*=(const Vector2& right) {
            *this = *this * right;
        }

        CH_FORCEINLINE Vector2 operator*(f32 s) const {
            return Vector2(x * s, y * s);
        }

        CH_FORCEINLINE void operator*=(f32 s) {
            *this = *this * s;
        }

        CH_FORCEINLINE Vector2 operator/(const Vector2& right) const {
            return Vector2(x / right.x, y / right.y);
        }

        CH_FORCEINLINE void operator/=(const Vector2& right) {
            *this = *this / right;
        }

        CH_FORCEINLINE Vector2 operator/(f32 s) const{
            return Vector2(x / s, y / s);
        }

        CH_FORCEINLINE void operator/=(f32 s) {
            *this = *this / s;
        }

        CH_FORCEINLINE f32 length_squared() const {
            return x * x + y * y;
        }

        CH_FORCEINLINE f32 length() const {
            return ch::sqrt(length_squared());
        }

        CH_FORCEINLINE void normalize() {
            const f32 l = length();
            if (l > 0.f) {
                *this /= l;
            }
        }
        
        CH_FORCEINLINE Vector2 get_normalized() const {
            Vector2 result = *this;
            result.normalize();
            return result;
        }

        CH_FORCEINLINE bool is_normalized() const {
            return get_normalized().length() == 1.f;
        }

        CH_FORCEINLINE f32 dot(const Vector2& other) const {
            return x * other.x + y * other.y;
        }
	};

    struct Vector3 {
        union {
            struct { f32 x, y, z; };
            struct { f32 u, v, _; };
            struct { f32 r, g, b; };
            struct { s32 ix, iy, iz; };
            struct { ch::Vector2 xy; f32 z; };
            f32 xyz[3];
            f32 uv[3];
        };

        Vector3() : x(0.f), y(0.f), z(0.f) {}
        Vector3(f32 xyz) : x(xyz), y(xyz), z(xyz) {}
        Vector3(s32 ixyz) : ix(ixyz), iy(ixyz), iz(ixyz) {}
        Vector3(f32 _x, f32 _y, f32 _z) : x(_x), y(_y), z(_z) {}

        explicit operator bool() const { return x != 0.f || y != 0.f || z != 0.f; }

        bool operator==(const Vector3& right) const {
            return x == right.x && y == right.y && z == right.z;
        }

        bool operator!=(const Vector3& right) const {
            return !(*this == right);
        }

        CH_FORCEINLINE Vector3 operator+(const Vector3& right) const {
            return Vector3(x + right.x, y + right.y, z + right.z);
        }

        CH_FORCEINLINE void operator+=(const Vector3& right) {
            *this = *this + right;
        }

        CH_FORCEINLINE Vector3 operator+(f32 s) const {
            return Vector3(x + s, y + s, z + s);
        }

        CH_FORCEINLINE void operator+=(f32 s) {
            *this = *this + s;
        }

        CH_FORCEINLINE Vector3 operator-(const Vector3& right) const {
            return Vector3(x - right.x, y - right.y, z - right.z);
        }

        CH_FORCEINLINE void operator-=(const Vector3& right) {
            *this = *this - right;
        }

        CH_FORCEINLINE Vector3 operator-(f32 s) const {
            return Vector3(x - s, y - s, z - s);
        }

        CH_FORCEINLINE void operator-=(f32 s) {
            *this = *this - s;
        }

        CH_FORCEINLINE Vector3 operator*(const Vector3& right) const {
            return Vector3(x * right.x, y * right.y, z * right.z);
        }

        CH_FORCEINLINE void operator*=(const Vector3& right) {
            *this = *this * right;
        }

        CH_FORCEINLINE Vector3 operator*(f32 s) const {
            return Vector3(x * s, y * s, z * s);
        }

        CH_FORCEINLINE void operator*=(f32 s) {
            *this = *this * s;
        }

        CH_FORCEINLINE Vector3 operator/(const Vector3& right) const {
            return Vector3(x / right.x, y / right.y, z / right.z);
        }

        CH_FORCEINLINE void operator/=(const Vector3& right) {
            *this = *this / right;
        }

        CH_FORCEINLINE Vector3 operator/(f32 s) const {
            return Vector3(x / s, y / s, z / s);
        }

        CH_FORCEINLINE void operator/=(f32 s) {
            *this = *this / s;
        }

        CH_FORCEINLINE f32 length_squared() const {
            return x * x + y * y + z * z;
        }

        CH_FORCEINLINE f32 length() const {
            return ch::sqrt(length_squared());
        }

        CH_FORCEINLINE void normalize() {
            const f32 l = length();
            if (l > 0.f) {
                *this /= l;
            }
        }

        CH_FORCEINLINE Vector3 get_normalized() const {
            Vector3 result = *this;
            result.normalize();
            return result;
        }

        CH_FORCEINLINE bool is_normalized() const {
            return get_normalized().length() == 1.f;
        }

        CH_FORCEINLINE f32 dot(const Vector3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        CH_FORCEINLINE Vector3 cross(const Vector3& other) const {
            Vector3 result;
            result.x = y * other.z - z * other.y;
            result.y = z * other.x - x * other.z;
            result.z = x * other.y - y * other.x;
            return result;
        }
    };

	// @TODO(CHall): Finish
	struct Vector4 {
		union {
			struct { f32 x, y, z, w; };
			struct { f32 r, g, b, a; };
			struct { s32 ix, iy, iz, iw; };
			struct { ch::Vector2 xy; ch::Vector2 zw; };
			struct { ch::Vector3 xyz; f32 w; };
			f32 xyzw[4];
			f32 rgba[4];
		};
	};

	struct Quaternion {
		f32 x, y, z, w;
	};


}