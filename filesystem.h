#pragma once

#include "string.h"
#include "os.h"
#include "types.h"
#include "array.h"

#if CH_PLATFORM_WINDOWS
#include "win32/filesystem_win32.h"
#endif

namespace ch {

	struct Path {
		tchar data[ch::max_path + 1];
		usize count = 0;
		usize allocated = ch::max_path + 1;

		Path() = default;
		Path(const tchar* in_path);

		CH_FORCEINLINE operator const tchar* const() { return data; }
		CH_FORCEINLINE tchar& operator[](usize index) {
			assert(index < count);
			return data[index];
		}
		CH_FORCEINLINE tchar operator[](usize index) const {
			assert(index < count);
			return data[index];
		}

		CH_FORCEINLINE operator bool() const { return count > 0; }

		CH_FORCEINLINE bool operator==(const ch::Path& right) const {
			if (count != right.count) return false;

			for (usize i = 0; i < count; i++) {
				if (right[i] != data[i]) return false;
			}

			return true;
		}

		CH_FORCEINLINE bool operator!=(const ch::Path& right) const {
			return !(*this == right);
		}

		CH_FORCEINLINE void clear() {
			count = 0;
			data[0] = 0;
		}

		void append(const tchar* ap);
		void remove_until_directory();
		ch::String get_extension();

		bool is_relative() const;
		CH_FORCEINLINE bool is_absolute() const { return !is_relative(); }
	};
    // @TODO(CHall): Add in more write functions
	struct Stream {
        OS_Stream_Handle os_handle;

		explicit operator bool() const { return os_handle; }

		Stream() : os_handle(nullptr) {}
		Stream(OS_Stream_Handle handle) : os_handle(handle) {}

        Stream& write_raw(const void* ptr, usize size);
        Stream& operator<<(const tchar* c_str);
        Stream& operator<<(const tchar c);
		Stream& operator<<(bool b);
		Stream& operator<<(u8 b);
		Stream& operator<<(s8 b);
		Stream& operator<<(u16 s);
		Stream& operator<<(s16 s);
		Stream& operator<<(u32 uint);
		Stream& operator<<(s32 i);
		Stream& operator<<(u64 ulong);
		Stream& operator<<(s64 slong);
		Stream& operator<<(f32 f);
		Stream& operator<<(f64 d);
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

	struct File_Data {
		u8* data = nullptr;
		usize size = 0;
		ch::Allocator allocator;

		void free();
	};

	bool load_file_into_memory(const tchar* path, ch::File_Data* fd, ch::Allocator allocator = ch::context_allocator);

    ch::Path get_current_path();
	bool set_current_path(const tchar* path);
	ch::Path get_os_font_path();
	ch::Path get_app_path();

	enum Directory_Result_Type {
		DRT_Directory,
		DRT_File,
		DRT_Other,
	};

	struct Directory_Result {
		Directory_Result_Type type;
		
		u64 creation_time;
		u64 last_access_time;
		u64 last_write_time;

		tchar file_name[max_path];
		usize file_size;
	};

	struct Base_Directory_Iterator {
		Base_Directory_Iterator() {}

		void advance() {}
		bool can_advance() const {}

		Directory_Result get() {}
	};

	struct Recursive_Directory_Iterator {
		ch::Array<ch::Directory_Iterator> iterators;
		usize current_iterator;
		ch::Path current_path;

		Recursive_Directory_Iterator() : Recursive_Directory_Iterator(ch::get_current_path()) {}
		Recursive_Directory_Iterator(const ch::Path& path);
		~Recursive_Directory_Iterator();
		void advance();
		bool can_advance() const;

		Directory_Result get();
	};

}