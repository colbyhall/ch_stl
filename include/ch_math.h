#pragma once

#include "ch_types.h"

// @TODO(CHall): Finish math lib
// Vector2
// Vector3
// Vector4
// Quaternion
// Plane
// Matrix4x4
// Scalar Math

namespace ch {



	struct Vector2 {
		union {
			struct { f32 x, y; };
			struct { f32 u, v; };
			struct { s32 ix, iy; };
		};

		Vector2() : x(0.f), y(0.f) {}
		Vector2(f32 xy) : x(xy), y(xy) {}
		Vector2(s32 ixy) : ix(ixy), iy(ixy) {}
	};

}