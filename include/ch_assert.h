#pragma once

#include "ch_stl.h"

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