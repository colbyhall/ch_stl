#include "../time.h"

#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

struct SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
};
using LPSYSTEMTIME = SYSTEMTIME*;

extern "C" {
	DLL_IMPORT BOOL		 WINAPI QueryPerformanceFrequency(LARGE_INTEGER*);
	DLL_IMPORT BOOL		 WINAPI QueryPerformanceCounter(LARGE_INTEGER*);
	DLL_IMPORT ULONGLONG WINAPI GetTickCount64(VOID);
	DLL_IMPORT BOOL		 WINAPI FileTimeToSystemTime(const FILETIME*, LPSYSTEMTIME);
	DLL_IMPORT VOID		 WINAPI GetSystemTime(LPSYSTEMTIME);
	DLL_IMPORT VOID		 WINAPI GetLocalTime(LPSYSTEMTIME);
}

f64 ch::get_time_in_seconds() {
	static LARGE_INTEGER qpc_freq;
	static BOOL use_qpc = QueryPerformanceFrequency(&qpc_freq);
	static f64 freq = (f64)qpc_freq.QuadPart;

	if (use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return now.QuadPart / freq;
	} else {
        return GetTickCount64() / 1000.0;
    }
}

static ch::Date_Time dt_from_win32dt(const SYSTEMTIME& st) {
	ch::Date_Time result;
	result.year = st.wYear;
	result.month = st.wMonth;
	result.day_of_week = st.wDayOfWeek;
	result.day_of_month = st.wDay;
	result.hour = st.wHour;
	result.minute = st.wMinute;
	result.second = st.wSecond;
	result.milliseconds = st.wMilliseconds;
	
	return result;
}

bool ch::date_time_from_file_time(u64 file_time, Date_Time* out_time) {
	SYSTEMTIME st;
	ULARGE_INTEGER uli;
	uli.QuadPart = file_time;
	FILETIME ft = {uli.LowPart, uli.HighPart};
	if (!FileTimeToSystemTime(&ft, &st)) return false;

	*out_time = dt_from_win32dt(st);

	return true;
}

ch::Date_Time ch::get_system_time() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	return dt_from_win32dt(st);
}

ch::Date_Time ch::get_local_time() {
	SYSTEMTIME st;
	GetLocalTime(&st);
	return dt_from_win32dt(st);
}