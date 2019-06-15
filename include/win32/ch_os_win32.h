#pragma once

#define WIN32_MEAN_AND_LEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace ch {
    using OS_Window_Handle = HWND;
}