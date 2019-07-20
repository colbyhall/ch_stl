#include "../time.h"

#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

#define WIN32_MEAN_AND_LEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

f64 ch::get_ms_time() {
	static LARGE_INTEGER qpc_freq;
	static BOOL use_qpc = QueryPerformanceFrequency(&qpc_freq);
	static f64 freq = 1.0 / qpc_freq.QuadPart;

	if (use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return now.QuadPart * freq;
	} else return GetTickCount();
}