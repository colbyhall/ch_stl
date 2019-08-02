#include "../filesystem.h"

#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

#define far
#define near
#include <shlwapi.h>

bool ch::Path::is_relative() const {
	return PathIsRelative(data);
}

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

ch::Path ch::get_current_path() {
    ch::Path result;
    GetCurrentDirectory(MAX_PATH, result.data);
    result.count = ch::strlen(result.data);
	result.data[result.count] = 0;
    return result;
}

bool ch::set_current_path(const tchar* path) {
	return SetCurrentDirectory(path);
}

ch::Path ch::get_os_font_path() {
	ch::Path result;

	GetWindowsDirectory(result.data, MAX_PATH);
	result.count = ch::strlen(result.data);
	result.data[result.count] = 0;

	result.append(CH_TEXT("\\Fonts"));
	result.data[result.count] = 0;

	return result;
}

ch::Path ch::get_app_path() {
	ch::Path result;
	GetModuleFileName(NULL, result.data, MAX_PATH);
	result.count = ch::strlen(result.data);
	result.data[result.count] = 0;

	return result;
}


ch::Win32_Directory_Iterator::Win32_Directory_Iterator(const ch::Path& path) {

	ch::Path actual_path = path;
	actual_path.append(CH_TEXT("\\*"));

	file = FindFirstFile(actual_path, &find_data);

	// @NOTE(CHall): Skip . ..
	// advance();
	// advance();
}

ch::Win32_Directory_Iterator::Win32_Directory_Iterator() : ch::Win32_Directory_Iterator(ch::get_current_path()) {}

bool ch::Win32_Directory_Iterator::can_advance() const {
	return file != INVALID_HANDLE_VALUE;
}

void ch::Win32_Directory_Iterator::advance() {
	if (!FindNextFile(file, &find_data)) {
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
