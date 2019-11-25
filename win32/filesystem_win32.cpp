#include "../filesystem.h"

#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

struct OVERLAPPED {
	ULONG_PTR Internal;
	ULONG_PTR InternalHigh;
	union {
		struct {
			DWORD Offset;
			DWORD OffsetHigh;
		};
		PVOID Pointer;
	};

	HANDLE  hEvent;
};

using LPOVERLAPPED = OVERLAPPED*;

#define STD_INPUT_HANDLE    ((DWORD)-10)
#define STD_OUTPUT_HANDLE   ((DWORD)-11)
#define STD_ERROR_HANDLE    ((DWORD)-12)

struct SECURITY_ATTRIBUTES {
	DWORD nLength;
	LPVOID lpSecurityDescriptor;
	BOOL bInheritHandle;
};

using LPSECURITY_ATTRIBUTES = SECURITY_ATTRIBUTES*;

#define FILE_SHARE_READ                 0x00000001  
#define FILE_SHARE_WRITE                0x00000002  
#define FILE_SHARE_DELETE               0x00000004  
#define FILE_ATTRIBUTE_READONLY             0x00000001  
#define FILE_ATTRIBUTE_HIDDEN               0x00000002  
#define FILE_ATTRIBUTE_SYSTEM               0x00000004  
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
#define FILE_ATTRIBUTE_DEVICE               0x00000040  
#define FILE_ATTRIBUTE_NORMAL               0x00000080  
#define FILE_ATTRIBUTE_TEMPORARY            0x00000100  
#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200  
#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400  
#define FILE_ATTRIBUTE_COMPRESSED           0x00000800  
#define FILE_ATTRIBUTE_OFFLINE              0x00001000  
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000  
#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000  
#define FILE_ATTRIBUTE_INTEGRITY_STREAM     0x00008000  
#define FILE_ATTRIBUTE_VIRTUAL              0x00010000  
#define FILE_ATTRIBUTE_NO_SCRUB_DATA        0x00020000  
#define FILE_ATTRIBUTE_EA                   0x00040000  
#define FILE_ATTRIBUTE_PINNED               0x00080000  
#define FILE_ATTRIBUTE_UNPINNED             0x00100000  
#define FILE_ATTRIBUTE_RECALL_ON_OPEN       0x00040000  
#define FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS 0x00400000 

#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define VOLUME_NAME_DOS  0x0      //default
#define VOLUME_NAME_GUID 0x1
#define VOLUME_NAME_NT   0x2
#define VOLUME_NAME_NONE 0x4

#define FILE_NAME_NORMALIZED 0x0  //default
#define FILE_NAME_OPENED     0x8

#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2

extern "C" {
	DLL_IMPORT BOOL   WINAPI PathIsRelativeA(LPCSTR);
	DLL_IMPORT HANDLE WINAPI GetStdHandle(DWORD);
	DLL_IMPORT BOOL   WINAPI WriteFile(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
	DLL_IMPORT HANDLE WINAPI CreateFileA(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
	DLL_IMPORT DWORD  WINAPI GetFinalPathNameByHandleA(HANDLE, LPSTR, DWORD, DWORD);
	DLL_IMPORT BOOL   WINAPI ReadFile(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
	DLL_IMPORT DWORD  WINAPI SetFilePointer(HANDLE, LONG, PLONG, DWORD);
	DLL_IMPORT BOOL   WINAPI SetEndOfFile(HANDLE);
	DLL_IMPORT DWORD  WINAPI GetFileSize(HANDLE, LPDWORD);
	DLL_IMPORT BOOL   WINAPI GetFileTime(HANDLE, LPFILETIME, LPFILETIME, LPFILETIME);
	DLL_IMPORT DWORD  WINAPI GetCurrentDirectoryA(DWORD, LPSTR);
	DLL_IMPORT BOOL   WINAPI SetCurrentDirectoryA(LPCSTR);
	DLL_IMPORT UINT   WINAPI GetWindowsDirectoryA(LPSTR, UINT);
	DLL_IMPORT DWORD  WINAPI GetModuleFileNameA(HMODULE, LPSTR, DWORD);
	DLL_IMPORT HANDLE WINAPI FindFirstFileA(LPCSTR, LPWIN32_FIND_DATAA);
	DLL_IMPORT BOOL   WINAPI FindNextFileA(HANDLE, LPWIN32_FIND_DATAA);
}

bool ch::Path::is_relative() const {
	return PathIsRelativeA(data);
}

ch::Stream ch::std_out = GetStdHandle(STD_OUTPUT_HANDLE);
ch::Stream ch::std_err = GetStdHandle(STD_ERROR_HANDLE);

ch::Stream& ch::Stream::write_raw(const void* ptr, usize size) {
	WriteFile(os_handle, ptr, (DWORD)size, nullptr, nullptr);
	return *this;
}

ch::Stream& ch::Stream::operator<<(const char* c_str) {
	return write_raw(c_str, ch::strlen(c_str) * sizeof(char));
}

ch::Stream& ch::Stream::operator<<(const char c) {
    return write_raw(&c, sizeof(c));
}

bool ch::File::open(const char* path, u32 open_flags) {
    flags = open_flags;
    const bool read = open_flags & FO_Read;
    const bool write = open_flags & FO_Write;
    const bool binary = open_flags & FO_Binary;
    const bool lock = open_flags & FO_Lock;
    const bool create = open_flags & FO_Create;

    assert(read || write);
    DWORD desired_access = 0;
    if (read) desired_access |= GENERIC_READ;
    if (write) desired_access |= GENERIC_WRITE;

    DWORD creation = OPEN_EXISTING;
    if (create) creation = CREATE_NEW;

    os_handle = CreateFileA(path, desired_access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    is_open = os_handle != INVALID_HANDLE_VALUE;

	if (!is_open && create) {
		os_handle = CreateFileA(path, desired_access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		is_open = os_handle != INVALID_HANDLE_VALUE;
	}

    return is_open;
}

bool ch::File::close() {
    if (is_open && CloseHandle(os_handle)) {
        is_open = false;
    }

    return !is_open;
}

bool ch::File::get_full_path(ch::Path* out_path) const {
	assert(is_open);
	out_path->count = GetFinalPathNameByHandleA(os_handle, out_path->data, (DWORD)out_path->allocated, FILE_NAME_OPENED | VOLUME_NAME_NONE);
	return  *out_path;
}

void ch::File::read(void* dest, usize size) {
    assert(is_open);
    ReadFile(os_handle, dest, (DWORD)size, NULL, NULL);
}

void ch::File::seek_top() {
    assert(is_open);
    SetFilePointer(os_handle, NULL, NULL, FILE_BEGIN);
}

void ch::File::seek_bottom() {
    assert(is_open);
    SetFilePointer(os_handle, NULL, NULL, FILE_END);
}

void ch::File::seek(ssize amount) {
    assert(is_open);
	LARGE_INTEGER li;
	li.QuadPart = amount;
	// @NOTE(CHall): Gotta make the compiler happy
    SetFilePointer(os_handle, li.LowPart, (PLONG)(s64)li.HighPart, FILE_CURRENT);
}

void ch::File::set_end_of_file() {
    assert(is_open);
    SetEndOfFile(os_handle);
}

usize ch::File::size() const {
    assert(is_open);
    const DWORD file_size = GetFileSize(os_handle, NULL);

    return (usize)file_size;
}

u64 ch::File::get_last_write_time() const {
	assert(is_open);
	FILETIME creation_time;
	FILETIME access_time;
	FILETIME write_time;

	GetFileTime((HANDLE)os_handle, &creation_time, &access_time, &write_time);
	
	LARGE_INTEGER result;
	result.LowPart = write_time.dwLowDateTime;
	result.HighPart = write_time.dwHighDateTime;
	
	return result.QuadPart;
}

ch::Path ch::get_current_path() {
    ch::Path result;
    GetCurrentDirectoryA((DWORD)ch::max_path, result.data);
    result.count = ch::strlen(result.data);
	result.data[result.count] = 0;
    return result;
}

bool ch::set_current_path(const char* path) {
	return SetCurrentDirectoryA(path);
}

ch::Path ch::get_os_font_path() {
	ch::Path result;

	GetWindowsDirectoryA(result.data, (DWORD)ch::max_path);
	result.count = ch::strlen(result.data);
	result.data[result.count] = 0;

	result.append("Fonts");
	result.data[result.count] = 0;

	return result;
}

ch::Path ch::get_app_path() {
	ch::Path result;
	GetModuleFileNameA(NULL, result.data, (DWORD)ch::max_path);
	result.count = ch::strlen(result.data);
	result.data[result.count] = 0;

	return result;
}


ch::Win32_Directory_Iterator::Win32_Directory_Iterator(const ch::Path& path) {

	ch::Path actual_path = path;
	actual_path.append("\\*");

	file = FindFirstFileA(actual_path, &find_data);
}

ch::Win32_Directory_Iterator::Win32_Directory_Iterator() : ch::Win32_Directory_Iterator(ch::get_current_path()) {}

bool ch::Win32_Directory_Iterator::can_advance() const {
	return file != INVALID_HANDLE_VALUE;
}

void ch::Win32_Directory_Iterator::advance() {
	if (!FindNextFileA(file, &find_data)) {
		file = INVALID_HANDLE_VALUE;
	}
}

ch::Directory_Result ch::Win32_Directory_Iterator::get() const {
	assert(can_advance());

	ch::Directory_Result result;
	result.type = DRT_Other;

	switch (find_data.dwFileAttributes) {
	case FILE_ATTRIBUTE_DIRECTORY:
		result.type = DRT_Directory;
		break;
	case FILE_ATTRIBUTE_NORMAL:
		result.type = DRT_File;
		break;
	case FILE_ATTRIBUTE_ARCHIVE:
		result.type = DRT_File;
		break;
	}

	LARGE_INTEGER file_size;
	file_size.LowPart = find_data.nFileSizeLow;
	file_size.HighPart = find_data.nFileSizeHigh;
	result.file_size = file_size.QuadPart;

	auto FILETIME_to_u64 = [](FILETIME ft) -> u64 {
		ULARGE_INTEGER uli;
		uli.LowPart = ft.dwLowDateTime;
		uli.HighPart = ft.dwHighDateTime;
		return uli.QuadPart;
	};
	
	ch::mem_copy(result.file_name, find_data.cFileName, ch::max_path);
	result.creation_time = FILETIME_to_u64(find_data.ftCreationTime);
	result.last_access_time = FILETIME_to_u64(find_data.ftLastAccessTime);
	result.last_write_time = FILETIME_to_u64(find_data.ftLastWriteTime);

	return result;
}
