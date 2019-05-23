/*
    Colby Hall's single header standard library
    Heavily inspired from gb lib and stb
    This lib is under construction
    

HOW TO
    One C++ file must #define CH_IMPLEMENTATION before including this library
        #define CH_IMPLEMENTATION
        #include "ch_stl.h"

    Any other header or C++ file can just include the library
        #include "ch_stl.h"

LICENSE
    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or
    distribute this software, either in source code form or as a compiled
    binary, for any purpose, commercial or non-commercial, and by any
    means.

    In jurisdictions that recognize copyright laws, the author or authors
    of this software dedicate any and all copyright interest in the
    software to the public domain. We make this dedication for the benefit
    of the public at large and to the detriment of our heirs and
    successors. We intend this dedication to be an overt act of
    relinquishment in perpetuity of all present and future rights to this
    software under copyright law.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

    For more information, please refer to <http://unlicense.org/>

CREDIT
    Written by Colby Hall
    
TODO
    - hash functions
    - file PLATFORM
    - containers
        - array
        - static_array
        - hash_map
        - set
    - multithreading
        - thread
        - mutex
        - semaphore
    - allocators
        - arena
        - pool
        - stack
        - free list


VERSIONS
    0.02a added max and defer
    0.02  pointer types
    0.01b project restructure
    0.01a initial commit
*/

#ifndef CH_INCLUDE_H
#define CH_INCLUDE_H
    
/** PLATFORM MACROS **/

#if !defined(__cplusplus)
    #error ch_stl only supports c++
#endif

#if __cplusplus < 199711L
    #error ch_stl requires a c++11 compiler or greater
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
    #define CH_PLATFORM_64BIT 1
    #define CH_PLATFORM_32BIT 0
#else
    #define CH_PLATFORM_64BIT 0
    #define CH_PLATFORM_32BIT 1
#endif

#if defined(_WIN32) || defined(_WIN64)
	#define CH_PLATFORM_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
	#define CH_PLATFORM_OSX 1
#elif defined(__unix__)
	#define CH_PLATFORM_UNIX 1

	#if defined(__linux__)
		#define CH_PLATFORM_LINUX 1
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#define CH_PLATFORM_FREEBSD 1
	#else
		#error This UNIX operating system is not supported
	#endif
#else
	#error This operating system is not supported
#endif

#ifndef CH_PLATFORM_WINDOWS
    #define CH_PLATFORM_WINDOWS 0
#endif
#ifndef CH_PLATFORM_OSX
    #define CH_PLATFORM_OSX 0
#endif
#ifndef CH_PLATFORM_UNIX
    #define CH_PLATFORM_UNIX 0
#endif
#ifndef CH_PLATFORM_LINUX
    #define CH_PLATFORM_LINUX 0
#endif
#ifndef CH_PLATFORM_FREEBSD
    #define CH_PLATFORM_FREEBSD 0
#endif

#if defined(_MSC_VER)
	#define CH_COMPILER_MSVC 1
#elif defined(__GNUC__)
	#define CH_COMPILER_GCC 1
#elif defined(__clang__)
	#define CH_COMPILER_CLANG 1
#else
	#error Unknown compiler
#endif

#ifndef CH_COMPILER_MSVC
    #define CH_COMPILER_MSVC 0
#endif
#ifndef CH_COMPILER_GCC
    #define CH_COMPILER_GCC 0
#endif
#ifndef CH_COMPILER_CLANG
    #define CH_COMPILER_CLANG 0
#endif

/** HEADERS **/

#if CH_PLATFORM_WINDOWS
    #define NOMINMAX 1
    #define WIN32_LEAN_AND_MEAN 1
    #define WIN32_MEAN_AND_LEAN 1
    #define VC_EXTRALEAN 1
    // @TODO(CHall) Create some kind of smaller windows.h?
    #include <windows.h>
    #undef NOMINMAX
    #undef WIN32_LEAN_AND_MEAN
    #undef WIN32_MEAN_AND_LEAN
    #undef VC_EXTRALEAN
#endif

/** TYPES **/

using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using s8  = signed char;
using s16 = signed short;
using s32 = signed int;
using s64 = signed long long;

#define CH_STRINGIZE(x) CH_STRINGIZE2(x)
#define CH_STRINGIZE2(x) #x

#define CH_LINE_STRING CH_STRINGIZE(__LINE__)
#define CH_COMPILE_ERROR_BEGIN "ch_stl compile error on line "

#define CH_TYPE_SIZE_ERROR CH_COMPILE_ERROR_BEGIN CH_LINE_STRING
#define CH_TYPE_SIZE_COMPARE(a, b) static_assert(sizeof(a) == sizeof(b), CH_TYPE_SIZE_ERROR)
#define CH_TYPE_SIZE_LITERAL(a, b) static_assert(sizeof(a) == b, CH_TYPE_SIZE_ERROR)

CH_TYPE_SIZE_COMPARE(u8,  s8);
CH_TYPE_SIZE_COMPARE(u16, s16);
CH_TYPE_SIZE_COMPARE(u32, s32);
CH_TYPE_SIZE_COMPARE(u64, s64);

CH_TYPE_SIZE_LITERAL(u8,  1);
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

template <typename F>
struct Priv_Defer {
    F f;
    Priv_Defer(F f) : f(f) {}
    ~Priv_Defer() { f(); }
};

template <typename F>
Priv_Defer<F> defer_func(F f) {
    return Priv_Defer<F>(f);
}

#define CH_DEFER_1(x, y)    x##y
#define CH_DEFER_2(x, y)    CH_DEFER_1(x, y)
#define CH_DEFER_3(x)       CH_DEFER_2(x, __COUNTER__)
#define defer(code)         auto CH_DEFER_3(_defer_) = defer_func([&](){code;})

/* ASSERT */

#define assert(cond) if (!(cond)) (*(void*)0)

/* MEMORY */

namespace Memory {
    void copy(void* dest, const void* src, usize size);
    void set(void* ptr, usize size, u8 c);
    void zero(void* ptr, usize size);
}

/* IMPLEMENTATION */

void Memory::copy(void* dest, const void* src, usize size) {
    const u8* casted_src = (u8*)src;
    u8* casted_dest = (u8*)dest;
    
    for(usize i = 0; i < size; i++) {
        casted_dest[i] = casted_src[i];
    }
}

void Memory::set(void* ptr, usize size, u8 c) {
    u8* casted_ptr = (u8*)ptr;
    
    for (usize i = 0; i < size; i++) {
        casted_ptr[i] = c;
    }
}

void Memory::zero(void* ptr, usize size) {
    Memory::set(ptr, size, 0);
}

#endif /*CH_INCLUDE_H*/