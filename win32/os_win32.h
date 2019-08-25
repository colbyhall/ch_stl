#pragma once

#include "../types.h"

namespace ch {
	using OS_Handle = void*;
    using OS_Window_Handle = OS_Handle;
    using OS_Stream_Handle = OS_Handle;
    using OS_File_Handle = OS_Stream_Handle;

	bool get_caret_blink_time(u32* out_ms);
}