#include "string.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <TCHAR.h>

usize ch::sprintf(tchar* buffer, const tchar* fmt, ...) {
	va_list args;
	va_start(args, fmt);
#if CH_UNICODE
	const usize result = (usize)wvsprintf(buffer, fmt, args);
#else
	const usize result = (usize)vsprintf(buffer, fmt, args);
#endif
	va_end(args);
	return result;
}

// @NOTE(Chall): shitty but it works
void ch::bytes_to_string(usize bytes, String* out_string) {
	auto print_to_out = [&](const tchar* fmt) {
		tchar buffer[1024];
		const usize size = ch::sprintf(buffer, fmt, bytes);
		out_string->reserve(size);
		ch::mem_copy(out_string->data, buffer, size);
		out_string->count = size;
	};

	if (bytes / 1024 > 0) {
		bytes /= 1024;

		if (bytes / 1024 > 0) {
			bytes /= 1024;
			if (bytes / 1024 > 0) {
				bytes /= 1024;
				print_to_out(CH_TEXT("%llugb"));
			}
			else {
				print_to_out(CH_TEXT("%llumb"));
			}
		} else {
			print_to_out(CH_TEXT("%llukb"));
		}
	} else {
		print_to_out(CH_TEXT("%llub"));
	}
}

bool ch::atof(const tchar* tstr, f32* f) {
	const f32 result = (f32)::_ttof(tstr);
	if (errno == EINVAL) {
		return false;
	}

	*f = result;
	return true;
}

bool ch::atoi(const tchar* tstr, s32* i) {
	const s32 result = ::_ttoi(tstr);
	if (errno == EINVAL) {
		return false;
	}

	*i = result;
	return true;
}
