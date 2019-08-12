#pragma once

#include "../os.h"

#define MAX_PATH 260

struct FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
};

struct WIN32_FIND_DATA {
	DWORD    dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD    nFileSizeHigh;
	DWORD    nFileSizeLow;
	DWORD    dwReserved0;
	DWORD    dwReserved1;
	TCHAR    cFileName[MAX_PATH];
	TCHAR    cAlternateFileName[14];
};

namespace ch {
	const usize max_path = MAX_PATH;

	struct Path;

	struct Win32_Directory_Iterator {
		WIN32_FIND_DATA find_data;
		HANDLE file;

		Win32_Directory_Iterator();
		Win32_Directory_Iterator(const ch::Path& path);

		bool can_advance() const;
		void advance();

		struct Directory_Result get() const;
	};

	using Directory_Iterator = Win32_Directory_Iterator;
}