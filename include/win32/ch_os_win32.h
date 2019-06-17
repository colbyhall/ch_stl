#pragma once

// @TODO(CHall): Maker a smaller windows.h?
#define WIN32_MEAN_AND_LEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace ch {
	using OS_Handle = HANDLE;
    using OS_Window_Handle = HWND;
    using OS_Stream_Handle = OS_Handle;
    using OS_File_Handle = OS_Stream_Handle;
}