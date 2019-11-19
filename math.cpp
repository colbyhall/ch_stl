#include "math.h"
#include "hash.h"
#include "filesystem.h"

#include <math.h>

f32 ch::sqrt(f32 s) {
    return ::sqrtf(s);
}

f32 ch::cos(f32 s) {
    return ::cosf(s);
}

f32 ch::sin(f32 s) {
    return ::sinf(s);
}

f32 ch::asin(f32 s) {
    return asinf(s);
}

f32 ch::tan(f32 s) {
    return tanf(s);
}

f32 ch::atan(f32 s) {
    return atanf(s);
}

f32 ch::atan2(f32 x, f32 y) {
    return atan2f(x, y);
}

ch::Matrix4 ch::Matrix4::operator*(const Matrix4& right) const {
	ch::Matrix4 result = {};
	for (s32 y = 0; y < 4; y++) {
		for (s32 x = 0; x < 4; x++) {
			f32 sum = 0.0f;
			for (s32 e = 0; e < 4; e++) {
				sum += elems[x + e * 4] * right[e + y * 4];
			}
			result[x + y * 4] = sum;
		}
	}
	return result;
}

// @NOTE(Chall): I wrote this in HS idk if its bad. its late
ch::Vector4 ch::Matrix4::operator*(const ch::Vector4& right) const {
	return ch::Vector4(
		rows[0].x * right.x + rows[0].y * right.y + rows[0].z * right.z + rows[0].w * right.w,
		rows[1].x * right.x + rows[1].y * right.y + rows[1].z * right.z + rows[1].w * right.w,
		rows[2].x * right.x + rows[2].y * right.y + rows[2].z * right.z + rows[2].w * right.w,
		rows[3].x * right.x + rows[3].y * right.y + rows[3].z * right.z + rows[3].w * right.w
	);
}

// @NOTE(CHall): How bad is this
ch::Matrix4 ch::Matrix4::inverse() {
	ch::Matrix4 result = *this;

	f32 temp[16];

	temp[0] = result.elems[5] * result.elems[10] * result.elems[15] -
		result.elems[5] * result.elems[11] * result.elems[14] -
		result.elems[9] * result.elems[6] * result.elems[15] +
		result.elems[9] * result.elems[7] * result.elems[14] +
		result.elems[13] * result.elems[6] * result.elems[11] -
		result.elems[13] * result.elems[7] * result.elems[10];

	temp[4] = -result.elems[4] * result.elems[10] * result.elems[15] +
		result.elems[4] * result.elems[11] * result.elems[14] +
		result.elems[8] * result.elems[6] * result.elems[15] -
		result.elems[8] * result.elems[7] * result.elems[14] -
		result.elems[12] * result.elems[6] * result.elems[11] +
		result.elems[12] * result.elems[7] * result.elems[10];

	temp[8] = result.elems[4] * result.elems[9] * result.elems[15] -
		result.elems[4] * result.elems[11] * result.elems[13] -
		result.elems[8] * result.elems[5] * result.elems[15] +
		result.elems[8] * result.elems[7] * result.elems[13] +
		result.elems[12] * result.elems[5] * result.elems[11] -
		result.elems[12] * result.elems[7] * result.elems[9];

	temp[12] = -result.elems[4] * result.elems[9] * result.elems[14] +
		result.elems[4] * result.elems[10] * result.elems[13] +
		result.elems[8] * result.elems[5] * result.elems[14] -
		result.elems[8] * result.elems[6] * result.elems[13] -
		result.elems[12] * result.elems[5] * result.elems[10] +
		result.elems[12] * result.elems[6] * result.elems[9];

	temp[1] = -result.elems[1] * result.elems[10] * result.elems[15] +
		result.elems[1] * result.elems[11] * result.elems[14] +
		result.elems[9] * result.elems[2] * result.elems[15] -
		result.elems[9] * result.elems[3] * result.elems[14] -
		result.elems[13] * result.elems[2] * result.elems[11] +
		result.elems[13] * result.elems[3] * result.elems[10];

	temp[5] = result.elems[0] * result.elems[10] * result.elems[15] -
		result.elems[0] * result.elems[11] * result.elems[14] -
		result.elems[8] * result.elems[2] * result.elems[15] +
		result.elems[8] * result.elems[3] * result.elems[14] +
		result.elems[12] * result.elems[2] * result.elems[11] -
		result.elems[12] * result.elems[3] * result.elems[10];

	temp[9] = -result.elems[0] * result.elems[9] * result.elems[15] +
		result.elems[0] * result.elems[11] * result.elems[13] +
		result.elems[8] * result.elems[1] * result.elems[15] -
		result.elems[8] * result.elems[3] * result.elems[13] -
		result.elems[12] * result.elems[1] * result.elems[11] +
		result.elems[12] * result.elems[3] * result.elems[9];

	temp[13] = result.elems[0] * result.elems[9] * result.elems[14] -
		result.elems[0] * result.elems[10] * result.elems[13] -
		result.elems[8] * result.elems[1] * result.elems[14] +
		result.elems[8] * result.elems[2] * result.elems[13] +
		result.elems[12] * result.elems[1] * result.elems[10] -
		result.elems[12] * result.elems[2] * result.elems[9];

	temp[2] = result.elems[1] * result.elems[6] * result.elems[15] -
		result.elems[1] * result.elems[7] * result.elems[14] -
		result.elems[5] * result.elems[2] * result.elems[15] +
		result.elems[5] * result.elems[3] * result.elems[14] +
		result.elems[13] * result.elems[2] * result.elems[7] -
		result.elems[13] * result.elems[3] * result.elems[6];

	temp[6] = -result.elems[0] * result.elems[6] * result.elems[15] +
		result.elems[0] * result.elems[7] * result.elems[14] +
		result.elems[4] * result.elems[2] * result.elems[15] -
		result.elems[4] * result.elems[3] * result.elems[14] -
		result.elems[12] * result.elems[2] * result.elems[7] +
		result.elems[12] * result.elems[3] * result.elems[6];

	temp[10] = result.elems[0] * result.elems[5] * result.elems[15] -
		result.elems[0] * result.elems[7] * result.elems[13] -
		result.elems[4] * result.elems[1] * result.elems[15] +
		result.elems[4] * result.elems[3] * result.elems[13] +
		result.elems[12] * result.elems[1] * result.elems[7] -
		result.elems[12] * result.elems[3] * result.elems[5];

	temp[14] = -result.elems[0] * result.elems[5] * result.elems[14] +
		result.elems[0] * result.elems[6] * result.elems[13] +
		result.elems[4] * result.elems[1] * result.elems[14] -
		result.elems[4] * result.elems[2] * result.elems[13] -
		result.elems[12] * result.elems[1] * result.elems[6] +
		result.elems[12] * result.elems[2] * result.elems[5];

	temp[3] = -result.elems[1] * result.elems[6] * result.elems[11] +
		result.elems[1] * result.elems[7] * result.elems[10] +
		result.elems[5] * result.elems[2] * result.elems[11] -
		result.elems[5] * result.elems[3] * result.elems[10] -
		result.elems[9] * result.elems[2] * result.elems[7] +
		result.elems[9] * result.elems[3] * result.elems[6];

	temp[7] = result.elems[0] * result.elems[6] * result.elems[11] -
		result.elems[0] * result.elems[7] * result.elems[10] -
		result.elems[4] * result.elems[2] * result.elems[11] +
		result.elems[4] * result.elems[3] * result.elems[10] +
		result.elems[8] * result.elems[2] * result.elems[7] -
		result.elems[8] * result.elems[3] * result.elems[6];

	temp[11] = -result.elems[0] * result.elems[5] * result.elems[11] +
		result.elems[0] * result.elems[7] * result.elems[9] +
		result.elems[4] * result.elems[1] * result.elems[11] -
		result.elems[4] * result.elems[3] * result.elems[9] -
		result.elems[8] * result.elems[1] * result.elems[7] +
		result.elems[8] * result.elems[3] * result.elems[5];

	temp[15] = result.elems[0] * result.elems[5] * result.elems[10] -
		result.elems[0] * result.elems[6] * result.elems[9] -
		result.elems[4] * result.elems[1] * result.elems[10] +
		result.elems[4] * result.elems[2] * result.elems[9] +
		result.elems[8] * result.elems[1] * result.elems[6] -
		result.elems[8] * result.elems[2] * result.elems[5];

	f32 determinant = result.elems[0] * temp[0] + result.elems[1] * temp[4] + result.elems[2] * temp[8] + result.elems[3] * temp[12];
	determinant = 1.0f / determinant;

	for (u8 i = 0; i < 4 * 4; i++) {
		result.elems[i] = temp[i] * determinant;
	}

	return result;
}

void ch::Matrix4::operator*=(const Matrix4& right) {
	*this = *this * right;
}

ch::Stream& ch::operator<<(ch::Stream &stream, Vector2 vec) {
	stream << '{' << vec.x << ", " << vec.y << "}\n";
	return stream;
}

ch::Stream& ch::operator<<(Stream& stream, const Color& color) {
	stream << color.r << (char)' ' << color.g << (char)' ' << color.b << (char)' ' << color.a;
	return stream;
}

ch::Matrix4 ch::identity() {
	ch::Matrix4 result = {};
	result[0 + 0 * 4] = 1.0f;
	result[1 + 1 * 4] = 1.0f;
	result[2 + 2 * 4] = 1.0f;
	result[3 + 3 * 4] = 1.0f;
	return result;
}

ch::Matrix4 ch::ortho(f32 left, f32 right, f32 top, f32 bottom, f32 far, f32 near) {
	Matrix4 result = ch::identity();

	result.elems[0 + 0 * 4] = 2.f / (right - left);
	result.elems[1 + 1 * 4] = 2.f / (top - bottom);
	result.elems[2 + 2 * 4] = -2.f / (far - near);

	result.elems[0 + 3 * 4] = -((right + left) / (right - left));
	result.elems[1 + 3 * 4] = -((top + bottom) / (top - bottom));
	result.elems[2 + 3 * 4] = -((far + near) / (far - near));

	return result;
}

ch::Matrix4 ch::ortho(f32 size, f32 aspect_ratio, f32 far, f32 near) {
	const float right = size * aspect_ratio;
	const float left = -right;

	const float top = size;
	const float bottom = -top;

	return ch::ortho(left, right, top, bottom, far, near);
}

ch::Matrix4 ch::perspective(f32 fov, f32 aspect_ratio, f32 far, f32 near) {
	ch::Matrix4 result = ch::identity();
	const float q = 1.f / ch::tan(fov * 0.5f * ch::to_rad);
	const float a = q / aspect_ratio;
	const float b = (near + far) / (near - far);
	const float c = (2.f * near * far) / (near - far);

	result[0 + 0 * 4] = a;
	result[1 + 1 * 4] = q;
	result[2 + 2 * 4] = b;
	result[3 + 2 * 4] = -1;
	result[2 + 3 * 4] = c;

	return result;
}

ch::Matrix4 ch::translate(const Vector3& pos) {
	ch::Matrix4 result = ch::identity();
	result[0 + 3 * 4] = pos.x;
	result[1 + 3 * 4] = pos.y;
	result[2 + 3 * 4] = pos.z;
	return result;
}

ch::Matrix4 ch::scale(const Vector3& scale) {
	ch::Matrix4 result = ch::identity();
	result[0 + 0 * 4] = scale.x;
	result[1 + 1 * 4] = scale.y;
	result[2 + 2 * 4] = scale.z;
	return result;
}

u64 hash(ch::Vector2 v) {
	return ch::fnv1_hash(&v, sizeof(ch::Vector2));
}
