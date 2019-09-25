#include "string.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>

usize ch::sprintf(char* buffer, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	const usize result = (usize)vsprintf(buffer, fmt, args);
	va_end(args);
	return result;
}

// @NOTE(Chall): shitty but it works
void ch::bytes_to_string(usize bytes, String* out_string) {
	auto print_to_out = [&](const char* fmt) {
		char buffer[1024];
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
				print_to_out("%llugb");
			} else {
				print_to_out("%llumb");
			}
		} else {
			print_to_out("%llukb");
		}
	} else {
		print_to_out("%llub");
	}
}

bool ch::atof(const char* tstr, f32* f) {
	const f32 result = (f32)::atof(tstr);
	if (errno == EINVAL) {
		return false;
	}

	*f = result;
	return true;
}

bool ch::atoi(const char* tstr, s32* i) {
	const s32 result = ::atoi(tstr);
	if (errno == EINVAL) {
		return false;
	}

	*i = result;
	return true;
}
