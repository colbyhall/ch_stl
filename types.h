#pragma once

#include "ch_stl.h"

using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using s8  = signed char;
using s16 = signed short;
using s32 = signed int;
using s64 = signed long long;

#if CH_UNICODE
using tchar = wchar_t;
#else
using tchar = char;
#endif

#define CH_STRINGIZE(x) CH_STRINGIZE2(x)
#define CH_STRINGIZE2(x) #x

#define CH_LINE_STRING CH_STRINGIZE(__LINE__)
#define CH_COMPILE_ERROR_BEGIN "ch_stl compile error on line "

#define CH_TYPE_SIZE_ERROR CH_COMPILE_ERROR_BEGIN CH_LINE_STRING
#define CH_TYPE_SIZE_COMPARE(a, b) static_assert(sizeof(a) == sizeof(b), CH_TYPE_SIZE_ERROR)
#define CH_TYPE_SIZE_LITERAL(a, b) static_assert(sizeof(a) == b, CH_TYPE_SIZE_ERROR)

CH_TYPE_SIZE_COMPARE(u8, s8);
CH_TYPE_SIZE_COMPARE(u16, s16);
CH_TYPE_SIZE_COMPARE(u32, s32);
CH_TYPE_SIZE_COMPARE(u64, s64);

CH_TYPE_SIZE_LITERAL(u8, 1);
CH_TYPE_SIZE_LITERAL(u16, 2);
CH_TYPE_SIZE_LITERAL(u32, 4);
CH_TYPE_SIZE_LITERAL(u64, 8);

#if CH_PLATFORM_64BIT
using usize = u64;
using ssize = s64;
#else
using usize = u32;
using ssize = s32;
#endif

using f32 = float;
using f64 = double;

CH_TYPE_SIZE_LITERAL(f32, 4);
CH_TYPE_SIZE_LITERAL(f64, 8);

#define U8_MIN 0u
#define U8_MAX 0xffu
#define U16_MIN 0u
#define U16_MAX 0xffffu
#define U32_MIN 0u
#define U32_MAX 0xffffffffu
#define U64_MIN 0ull
#define U64_MAX 0xffffffffffffffffull

#define S8_MIN (-0x7f - 1)
#define S8_MAX 0x7f
#define S16_MIN (-0x7fff - 1)
#define S16_MAX 0x7fff
#define S32_MIN (-0x7fffffff - 1)
#define S32_MAX 0x7fffffff
#define S64_MIN (-0x7fffffffffffffffll - 1)
#define S64_MAX 0x7fffffffffffffffll

#define F32_MIN 1.17549435e-38f
#define F32_MAX 3.40282347e+38f

#define F64_MIN 2.2250738585072014e-308
#define F64_MAX 1.7976931348623157e+308

#undef CH_STRINGIZE
#undef CH_STRINGIZE2

#undef CH_LINE_STRING
#undef CH_COMPILE_ERROR_BEGIN

#undef CH_TYPE_SIZE_ERROR
#undef CH_TYPE_SIZE_COMPARE
#undef CH_TYPE_SIZE_LITERAL
