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

static const u8 utf8d[] = {
	// The first part of the table maps bytes to character classes that
	// to reduce the size of the transition table and create bitmasks.
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	 8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

	// The second part is a transition table that maps a combination
	// of a state of the automaton and a character class to a state.
	0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
	12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
	12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
	12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
	12,36,12,12,12,12,12,12,12,12,12,12,
};

u32 ch::utf8_decode(u32* state, u32* code_p, u32 byte) {
	u32 type = utf8d[byte];

	*code_p = (*state != ch::utf8_accept) ? (byte & 0x3fu) | (*code_p << 6) : (0xff >> type) & (byte);

	*state = utf8d[256 + *state + type];
	return *state;
}

s32 ch::utf8_encode(u32 c, void* dest, u32* errors) {
	*errors |= ((c << 11) == 0x1b) | (c > 0x10ffff) << 1 | ((c >> 1) == 0x7fff) << 2;
	char len = (c > 0x7f) + (c > 0x7ff) + (c > 0xffff);
	char* p = (char*)dest;

	char head = 0xf0e0c000 >> (len << 3);
	p[len] = 0x80 | c & 0x3f;
	p[len >> 1] = 0x80 | c >> 12 & 0x3f;
	p[1 << len >> 2] = 0x80 | c >> 6 & 0x3f;
	p[0] = head | c >> len * 6 & ~head >> 1;
	return len + 1;
}

usize ch::utf8_strlen(const char* str) {
	usize result = 0;
	while (*str++) result += (*str & 0xc0) != 0x80;
	return result;
}

u32 ch::utf32_to_utf8(const ch::String_UTF32& utf32, ch::String_UTF8* out_utf8) {
	assert(out_utf8);
	if (!utf32) return 0;

	u8 dummy[4];
	u32 errors = 0;

	usize reservation_amount = 0;
	for (usize i = 0; i < utf32.count; i += 1) {
		reservation_amount += ch::utf8_encode(utf32[i], dummy, &errors);
	}
	
	if (out_utf8->allocated < reservation_amount) {
		out_utf8->reserve(reservation_amount - out_utf8->allocated);
	}
	errors = 0;

	char* cur = out_utf8->data;
	char* end = out_utf8->data + out_utf8->allocated;
	ssize i = -1;
	while ((cur += utf8_encode(utf32[i += 1], cur, &errors)) < end);
	out_utf8->count = utf32.count;
	out_utf8->num_errors = errors;
	
	return errors;
}

bool ch::utf8_to_utf32(const ch::String_UTF8& utf8, ch::String_UTF32* out_utf32) {
	assert(out_utf32);
	if (!utf8.allocated) return true;

	if (out_utf32->allocated < utf8.allocated) {
		out_utf32->reserve(utf8.allocated - out_utf32->allocated);
	}

	u32 decoder_state = ch::utf8_accept;
	u32 codepoint = 0;
	for (usize i = 0; i < utf8.allocated; i += 1) {
		const u8 c = (u8)utf8.data[i];
		utf8_decode(&decoder_state, &codepoint, c);
		
		if (decoder_state == ch::utf8_reject) return false;
		if (decoder_state != ch::utf8_accept) continue;

		// @TODO(CHall): Should do a push?
		out_utf32->data[out_utf32->count] = codepoint;
		out_utf32->count += 1;
	}

	return true;
}

ch::UTF8_Iterator::UTF8_Iterator(const char* _buffer, usize _size) : buffer(_buffer), size(_size) {
	assert(buffer && size);
}

bool ch::UTF8_Iterator::can_advance() const {
	return index < size;
}

void ch::UTF8_Iterator::advance() {
	assert(can_advance());

	index += 1;
}

u32 ch::UTF8_Iterator::get() {
	for (; index < size; index += 1) {
		const u8 c = (u8)buffer[index];
		utf8_decode(&decoder_state, &codepoint, c);

		if (decoder_state == ch::utf8_reject) {
			return '?';
		}

		if (decoder_state != ch::utf8_accept) continue;

		break;
	}
	return codepoint;
}
