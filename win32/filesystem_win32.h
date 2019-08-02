#pragma once

#define WIN32_MEAN_AND_LEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#undef near
#undef far

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