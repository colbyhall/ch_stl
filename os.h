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

	struct Path;

	struct Library {
		OS_Library_Handle handle;

		CH_FORCEINLINE operator bool() const {
			return handle;
		}
		
		void* get_function(const tchar* function_name);

		template <typename T>
		T get_function(const tchar* function_name) {
			return (T)get_function(function_name);
		}

		void free();
	};

	bool load_library(const ch::Path& path, ch::Library* lib);

}