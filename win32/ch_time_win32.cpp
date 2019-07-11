#include "../ch_time.h"

#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

#define WIN32_MEAN_AND_LEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

u64 ch::get_ms_time() {
	static LARGE_INTEGER qpc_freq;
	static BOOL use_qpc = QueryPerformanceFrequency(&qpc_freq);

	if (use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / qpc_freq.QuadPart;
	} else return GetTickCount();
}