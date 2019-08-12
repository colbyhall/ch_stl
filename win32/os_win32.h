#pragma once

#include "../types.h"

using HANDLE = void*;
using DWORD = unsigned long;
using TCHAR = tchar;

namespace ch {
	using OS_Handle = HANDLE;
    using OS_Window_Handle = OS_Handle;
    using OS_Stream_Handle = OS_Handle;
    using OS_File_Handle = OS_Stream_Handle;
}