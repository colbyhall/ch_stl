#pragma once

#include "string.h"

#if CH_PLATFORM_WINDOWS
#include "win32/os_win32.h"
#else
#error Platform not supported
#endif

namespace ch {
	bool copy_to_clipboard(ch::OS_Window_Handle window_handle, const void* buffer, usize size);
	bool copy_from_clipboard(ch::OS_Window_Handle window_handle, ch::String* out_str);
}