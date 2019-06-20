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
