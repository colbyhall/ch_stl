#include <ch_filesystem.h>


#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

#define WIN32_MEAN_AND_LEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

ch::OS_Stream ch::std_out = GetStdHandle(STD_OUTPUT_HANDLE);
ch::OS_Stream ch::std_err = GetStdHandle(STD_ERROR_HANDLE);

ch::OS_Stream& ch::OS_Stream::write_raw(const void* ptr, usize size) {
	WriteFile((HANDLE)os_handle, ptr, (DWORD)size, nullptr, nullptr);
	return *this;
}

ch::OS_Stream& ch::OS_Stream::operator<<(const char* c_str) {
	return write_raw(c_str, ch::strlen(c_str));
}

