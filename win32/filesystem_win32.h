#pragma once

#include "../types.h"

#define MAX_PATH 260

struct FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
};
using LPFILETIME = FILETIME*;

struct WIN32_FIND_DATAA {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	DWORD dwReserved0;
	DWORD dwReserved1;
	CHAR   cFileName[MAX_PATH];
	CHAR   cAlternateFileName[14];
};
using LPWIN32_FIND_DATAA = WIN32_FIND_DATAA*;

namespace ch {
	const usize max_path = MAX_PATH;

	struct Path;

	struct Win32_Directory_Iterator {
		WIN32_FIND_DATAA find_data;
		HANDLE file;

		Win32_Directory_Iterator();
		Win32_Directory_Iterator(const ch::Path& path);

		bool can_advance() const;
		void advance();

		struct Directory_Result get() const;
	};

	using Directory_Iterator = Win32_Directory_Iterator;
}

#undef MAX_PATH