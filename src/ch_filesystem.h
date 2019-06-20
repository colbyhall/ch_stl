#pragma once

#include "ch_string.h"
#include "ch_os.h"
#include "ch_types.h"

namespace ch {
    // @TODO(CHall): Add in more write functions
	struct OS_Stream {
        OS_Stream_Handle os_handle;

		explicit operator bool() const { return os_handle; }

		OS_Stream() : os_handle(nullptr) {}
		OS_Stream(OS_Stream_Handle handle) : os_handle(handle) {}

		OS_Stream& write_raw(const void* ptr, usize size);
		OS_Stream& operator<<(const tchar* c_str);
        OS_Stream& operator<<(const tchar c);
	};

	extern OS_Stream std_out;
	extern OS_Stream std_err;

	enum File_Open_Flags {
		FO_Read,
		FO_Write,
		FO_Binary,
		FO_Lock
	};

	// @TODO(CHall): Finish this
    struct File : public OS_Stream {
		u32 flags;
		bool is_open;

		bool open(const tchar* path, u32 open_flags) {}
		bool close() {}
    };

    ch::String get_current_path();
    bool set_current_path(const ch::String& path);

}