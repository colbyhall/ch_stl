#pragma once

#include "../types.h"

using HANDLE = void*;
using HINSTANCE = HANDLE;
using HMODULE = HINSTANCE;
using HWND = HANDLE;

using BOOL = bool;
using CHAR = char;
using INT = s32;
using UINT = u32;

using LONG = long;
using PLONG = LONG*;

using LONGLONG = s64;
using ULONGLONG = u64;

#define VOID void
using LPVOID = VOID*;
using PVOID = VOID*;
using LPCVOID = const VOID*;

using LPSTR = char*;
using LPCSTR = const char*;

using WORD = u16;

using DWORD = u32;
using LPDWORD = DWORD*;

using ULONG_PTR = u64;
using LONG_PTR = s64;

using UINT_PTR = u64;

using FLOAT = float;
using INT64 = s64;
using INT32 = s32;
using USHORT = u16;

union LARGE_INTEGER {
	struct {
		DWORD LowPart;
		LONG HighPart;
	};
	struct u {
		DWORD LowPart;
		LONG HighPart;
	};
	LONGLONG QuadPart;
};
using PLARGE_INTEGER = LARGE_INTEGER*;

union ULARGE_INTEGER {
	struct {
		DWORD LowPart;
		DWORD HighPart;
	};
	struct u {
		DWORD LowPart;
		DWORD HighPart;
	};
	ULONGLONG QuadPart;
};

using HRESULT = LONG;

struct RECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
};

using BYTE = u8;

using SIZE_T = usize;

#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR) - 1)
#define WINAPI __stdcall

#define MAKEINTRESOURCEA(i) ((LPSTR)((ULONG_PTR)((WORD)(i))))

using LRESULT = LONG_PTR;
using WPARAM = UINT_PTR;
using LPARAM = LONG_PTR;

extern "C" {
	DLL_IMPORT BOOL CloseHandle(HANDLE);
	DLL_IMPORT HMODULE GetModuleHandleA(LPCSTR);
}

namespace ch {
	using OS_Handle = HANDLE;
	using OS_Window_Handle = OS_Handle;
	using OS_Stream_Handle = OS_Handle;
	using OS_Library_Handle = OS_Handle;

	bool get_caret_blink_time(u32* out_ms);
}