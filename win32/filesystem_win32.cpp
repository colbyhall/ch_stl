#include "../filesystem.h"

#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

#define WIN32_MEAN_AND_LEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

ch::Stream ch::std_out = GetStdHandle(STD_OUTPUT_HANDLE);
ch::Stream ch::std_err = GetStdHandle(STD_ERROR_HANDLE);

ch::Stream& ch::Stream::write_raw(const void* ptr, usize size) {
	WriteFile(os_handle, ptr, (DWORD)size, nullptr, nullptr);
	return *this;
}

ch::Stream& ch::Stream::operator<<(const tchar* c_str) {
	return write_raw(c_str, ch::strlen(c_str));
}

ch::Stream& ch::Stream::operator<<(const tchar c) {
    return write_raw(&c, 1);
}

bool ch::File::open(const tchar* path, u32 open_flags) {
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

    os_handle = CreateFile(path, desired_access, 0, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    is_open = os_handle != INVALID_HANDLE_VALUE;
    return is_open;
}

bool ch::File::close() {
    if (is_open && CloseHandle(os_handle)) {
        is_open = false;
    }

    return is_open;
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
    SetFilePointer(os_handle, NULL, (PLONG)amount, FILE_CURRENT);
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

ch::String ch::get_current_path() {
    ch::String result;
    result.reserve(MAX_PATH);
    GetCurrentDirectory(MAX_PATH, result.data);
    result.count = ch::strlen(result.data);
    return result;
}

bool ch::set_current_path(const ch::String& path) {
    return SetCurrentDirectory(path);
}

ch::String ch::get_os_font_path() {
	ch::String result;
	result.reserve(MAX_PATH);

	GetWindowsDirectory(result.data, MAX_PATH);
	result.count = ch::strlen(result.data);

	// @NOTE(CHall): Has to null terminate
	result.append(CH_TEXT("\\Fonts\0"));

	return result;
}
