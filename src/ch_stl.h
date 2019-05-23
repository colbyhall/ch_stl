#pragma once

/**
 * AUTHOR: Colby Hall
 * DATE: 5/22/2019
 * COMMENT: 
 */

#if !defined(BUILD_DEBUG)
#define BUILD_DEBUG 0
#endif
#if !defined(BUILD_RELEASE)
#define BUILD_RELEASE 0
#endif

#ifdef _WIN32
#define PLATFORM_WINDOWS 1
    #ifdef _WIN64
    #define PLATFORM_64BIT 1
    #define PLATFORM_32BIT 0
    #else
    #define PLATFORM_64BIT 0
    #define PLATFORM_32BIT 1
    #endif
#endif

#if PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#error This library is currently not supported on your platform
#endif

#if !defined(__cplusplus)
#error This library only supports c++
#endif

namespace ch {
    #if PLATFORM_64BIT
    using u8  = unsigned char;
    using u16 = unsigned short;
    using u32 = unsigned int;
    using u64 = unsigned long long;

    using s8  = char;
    using s16 = short;
    using s32 = int;
    using s64 = long long;

    using f32 = float;
    using f64 = double;

    using p_size = size_t;
    #endif
}