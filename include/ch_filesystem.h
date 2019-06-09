#pragma once

#include "ch_string.h"
#include "ch_types.h"

namespace ch {

	using Stream_Handle = void*;
	using File_Handle = Stream_Handle;

    // @TODO(CHall): Add in more write functions
	struct OS_Stream {
		Stream_Handle os_handle;

		explicit operator bool() const { return os_handle; }

		OS_Stream() : os_handle(nullptr) {}
		OS_Stream(Stream_Handle handle) : os_handle(handle) {}

		OS_Stream& write_raw(const void* ptr, usize size);
		OS_Stream& operator<<(const char* c_str);
	};

	extern OS_Stream std_out;
	extern OS_Stream std_err;

    struct File : public OS_Stream {
        
    };

}