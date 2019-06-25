#include "ch_math.h"

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

// @NOTE(CHall): How bad is this
ch::Matrix4 ch::Matrix4::inverse() {
	ch::Matrix4 result;
	result.elems[0] = elems[5] * elems[10] * elems[15] -
		elems[5] * elems[11] * elems[14] -
		elems[9] * elems[6] * elems[15] +
		elems[9] * elems[7] * elems[14] +
		elems[13] * elems[6] * elems[11] -
		elems[13] * elems[7] * elems[10];

	result.elems[4] = -elems[4] * elems[10] * elems[15] +
		elems[4] * elems[11] * elems[14] +
		elems[8] * elems[6] * elems[15] -
		elems[8] * elems[7] * elems[14] -
		elems[12] * elems[6] * elems[11] +
		elems[12] * elems[7] * elems[10];

	result.elems[8] = elems[4] * elems[9] * elems[15] -
		elems[4] * elems[11] * elems[13] -
		elems[8] * elems[5] * elems[15] +
		elems[8] * elems[7] * elems[13] +
		elems[12] * elems[5] * elems[11] -
		elems[12] * elems[7] * elems[9];

	result.elems[12] = -elems[4] * elems[9] * elems[14] +
		elems[4] * elems[10] * elems[13] +
		elems[8] * elems[5] * elems[14] -
		elems[8] * elems[6] * elems[13] -
		elems[12] * elems[5] * elems[10] +
		elems[12] * elems[6] * elems[9];

	result.elems[1] = -elems[1] * elems[10] * elems[15] +
		elems[1] * elems[11] * elems[14] +
		elems[9] * elems[2] * elems[15] -
		elems[9] * elems[3] * elems[14] -
		elems[13] * elems[2] * elems[11] +
		elems[13] * elems[3] * elems[10];

	result.elems[5] = elems[0] * elems[10] * elems[15] -
		elems[0] * elems[11] * elems[14] -
		elems[8] * elems[2] * elems[15] +
		elems[8] * elems[3] * elems[14] +
		elems[12] * elems[2] * elems[11] -
		elems[12] * elems[3] * elems[10];

	result.elems[9] = -elems[0] * elems[9] * elems[15] +
		elems[0] * elems[11] * elems[13] +
		elems[8] * elems[1] * elems[15] -
		elems[8] * elems[3] * elems[13] -
		elems[12] * elems[1] * elems[11] +
		elems[12] * elems[3] * elems[9];

	result.elems[13] = elems[0] * elems[9] * elems[14] -
		elems[0] * elems[10] * elems[13] -
		elems[8] * elems[1] * elems[14] +
		elems[8] * elems[2] * elems[13] +
		elems[12] * elems[1] * elems[10] -
		elems[12] * elems[2] * elems[9];

	result.elems[2] = elems[1] * elems[6] * elems[15] -
		elems[1] * elems[7] * elems[14] -
		elems[5] * elems[2] * elems[15] +
		elems[5] * elems[3] * elems[14] +
		elems[13] * elems[2] * elems[7] -
		elems[13] * elems[3] * elems[6];

	result.elems[6] = -elems[0] * elems[6] * elems[15] +
		elems[0] * elems[7] * elems[14] +
		elems[4] * elems[2] * elems[15] -
		elems[4] * elems[3] * elems[14] -
		elems[12] * elems[2] * elems[7] +
		elems[12] * elems[3] * elems[6];

	result.elems[10] = elems[0] * elems[5] * elems[15] -
		elems[0] * elems[7] * elems[13] -
		elems[4] * elems[1] * elems[15] +
		elems[4] * elems[3] * elems[13] +
		elems[12] * elems[1] * elems[7] -
		elems[12] * elems[3] * elems[5];

	result.elems[14] = -elems[0] * elems[5] * elems[14] +
		elems[0] * elems[6] * elems[13] +
		elems[4] * elems[1] * elems[14] -
		elems[4] * elems[2] * elems[13] -
		elems[12] * elems[1] * elems[6] +
		elems[12] * elems[2] * elems[5];

	result.elems[3] = -elems[1] * elems[6] * elems[11] +
		elems[1] * elems[7] * elems[10] +
		elems[5] * elems[2] * elems[11] -
		elems[5] * elems[3] * elems[10] -
		elems[9] * elems[2] * elems[7] +
		elems[9] * elems[3] * elems[6];

	result.elems[7] = elems[0] * elems[6] * elems[11] -
		elems[0] * elems[7] * elems[10] -
		elems[4] * elems[2] * elems[11] +
		elems[4] * elems[3] * elems[10] +
		elems[8] * elems[2] * elems[7] -
		elems[8] * elems[3] * elems[6];

	result.elems[11] = -elems[0] * elems[5] * elems[11] +
		elems[0] * elems[7] * elems[9] +
		elems[4] * elems[1] * elems[11] -
		elems[4] * elems[3] * elems[9] -
		elems[8] * elems[1] * elems[7] +
		elems[8] * elems[3] * elems[5];

	result.elems[15] = elems[0] * elems[5] * elems[10] -
		elems[0] * elems[6] * elems[9] -
		elems[4] * elems[1] * elems[10] +
		elems[4] * elems[2] * elems[9] +
		elems[8] * elems[1] * elems[6] -
		elems[8] * elems[2] * elems[5];

	return result;
}

void ch::Matrix4::operator*=(const Matrix4& right) {
	*this = *this * right;
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
