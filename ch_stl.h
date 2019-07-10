/*
    Colby Hall's standard library
    Heavily inspired from gb lib and stb libs
    This lib is under construction
    

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
        - fixed_array
        - hash_map
        - set
    - multithreading
        - thread
        - mutex
        - semaphore
    - allocators
        - pool
        - stack
        - free list


VERSIONS
    0.04b array initializer list
    0.04a memory, array test finished
    0.04  fixed issues with array and starting to setup testing
    0.03b finished more parts of the array struct
    0.03a started on container and new stuff
    0.03  allocator base
    0.02a added max and defer
    0.02  pointer types
    0.01b project restructure
    0.01a initial commit
*/

#pragma once
    

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

#if CH_COMPILER_MSVC
#define CH_FORCEINLINE __forceinline
#endif

#ifndef CH_FORCEINLINE
#define CH_FORCEINLINE inline
#endif

#ifdef UNICODE
#define CH_UNICODE 1
#endif

#ifndef CH_UNICODE
#define CH_UNICODE 0
#endif

#if CH_UNICODE
#define CH_TEXT(str) L##str
#else
#define CH_TEXT(str) str
#endif

#if CH_COMPILER_MSVC
#if _MSC_VER < 1300 
#define ch_debug_trap __asm int 3
#else
#define ch_debug_trap __debugbreak()
#endif
#else
#define ch_debug_trap __builtin_trap()
#endif

// @TODO(CHall): Finish this
#define assert(cond) if (!(cond)) ch_debug_trap

/**
 * This defer was created by Iganico Castano
 *
 * http://the-witness.net/news/2012/11/scopeexit-in-c11/
 */
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
