#pragma once

#include "ch_string.h"
#include "ch_os.h"
#include "ch_types.h"

namespace ch {
    // @TODO(CHall): Add in more write functions
	struct Stream {
        OS_Stream_Handle os_handle;

		explicit operator bool() const { return os_handle; }

		Stream() : os_handle(nullptr) {}
		Stream(OS_Stream_Handle handle) : os_handle(handle) {}

        Stream& write_raw(const void* ptr, usize size);
        Stream& operator<<(const tchar* c_str);
        Stream& operator<<(const tchar c);
	};

    extern Stream std_out;
    extern Stream std_err;

	enum File_Open_Flags {
		FO_Read = 0x01,
		FO_Write = 0x02,
		FO_Binary = 0x04,
		FO_Lock = 0x08,
        FO_Create = 0x10,
	};

	// @TODO(CHall): Finish this
    struct File : public Stream {
		u32 flags;
		bool is_open;

        bool open(const tchar* path, u32 open_flags);
        bool close();

        void read(void* dest, usize size);
        void seek_top();
        void seek_bottom();
        void seek(ssize amount);
        void set_end_of_file();
        usize size() const;
    };

    ch::String get_current_path();
    bool set_current_path(const ch::String& path);

}