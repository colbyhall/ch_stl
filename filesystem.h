#pragma once

#include "string.h"
#include "os.h"
#include "array.h"

#if CH_PLATFORM_WINDOWS
#include "win32/filesystem_win32.h"
#endif

namespace ch {

	struct Path {
		char data[ch::max_path + 1];
		usize count = 0;
		usize allocated = ch::max_path + 1;

		Path() = default;
		Path(const char* in_path);
		Path(const ch::String& s);

		CH_FORCEINLINE operator const char*() const { return data; }
		CH_FORCEINLINE char& operator[](usize index) {
			assert(index < count);
			return data[index];
		}
		CH_FORCEINLINE char operator[](usize index) const {
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

		void append(const char* ap, bool ensure_proper = true);
		void remove_until_directory();
		const ch::String get_extension();
		const ch::String get_filename(bool with_extension = false);

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
        Stream& operator<<(const char* c_str);
        Stream& operator<<(const char c);
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

		template <typename T>
		Stream& operator<<(ch::Base_String<T> str) {
			write_raw(str.data, str.count * sizeof(T));
			return *this;
		}
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

        bool open(const char* path, u32 open_flags);
        bool close();

		bool get_absolute_path(ch::Path* out_path) const;

        void read(void* dest, usize size);
        void seek_top();
        void seek_bottom();
        void seek(ssize amount);
        void set_end_of_file();
        usize size() const;

		u64 get_last_write_time() const;
    };

	struct File_Data {
		u8* data = nullptr;
		usize size = 0;
		ch::Allocator allocator;

		File_Data() : data(nullptr), size(0), allocator(ch::context_allocator) {}

		void free();
		ch::String to_string() const;
	};

	bool load_file_into_memory(const char* path, ch::File_Data* fd, ch::Allocator allocator = ch::context_allocator);

    ch::Path get_current_path();
	bool set_current_path(const char* path);
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

		char file_name[max_path];
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