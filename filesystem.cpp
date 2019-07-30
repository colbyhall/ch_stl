#include "filesystem.h"

// @HACK(CHall): We don't need to use cstl
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <stdio.h>

ch::Stream& ch::Stream::operator<<(bool b) {
	*this << (b ? CH_TEXT("true") : CH_TEXT("false"));
	return *this;
}

ch::Stream& ch::Stream::operator<<(u8 b) {
	tchar buffer[5];
#if CH_UNICODE
	swprintf(buffer, CH_TEXT("%u"), b);
#else
	sprintf(buffer, "%u", b);
#endif

	write_raw(buffer, ch::strlen(buffer));
	return *this;
}

ch::Stream& ch::Stream::operator<<(s8 b) {
	tchar buffer[5];
#if CH_UNICODE
	swprintf(buffer, CH_TEXT("%i"), b);
#else
	sprintf(buffer, "%i", b);
#endif

	write_raw(buffer, ch::strlen(buffer));
	return *this;
}

ch::Stream& ch::Stream::operator<<(u16 s) {
	tchar buffer[10];
#if CH_UNICODE
	swprintf(buffer, CH_TEXT("%u"), s);
#else
	sprintf(buffer, "%u", s);
#endif

	write_raw(buffer, ch::strlen(buffer));
	return *this;
}

ch::Stream& ch::Stream::operator<<(s16 s) {
	tchar buffer[10];
#if CH_UNICODE
	swprintf(buffer, CH_TEXT("%i"), s);
#else
	sprintf(buffer, "%i", s);
#endif

	write_raw(buffer, ch::strlen(buffer));
	return *this;
}

ch::Stream& ch::Stream::operator<<(u32 uint) {
	tchar buffer[32];
#if CH_UNICODE
	swprintf(buffer, CH_TEXT("%u"), uint);
#else
	sprintf(buffer, "%u", uint);
#endif

	write_raw(buffer, ch::strlen(buffer));
	return *this;
}

ch::Stream& ch::Stream::operator<<(s32 i) {
	tchar buffer[32];
#if CH_UNICODE
	swprintf(buffer, CH_TEXT("%i"), i);
#else
	sprintf(buffer, "%i", i);
#endif

	write_raw(buffer, ch::strlen(buffer));
	return *this;
}

ch::Stream& ch::Stream::operator<<(u64 ulong) {
	tchar buffer[64];
#if CH_UNICODE
	swprintf(buffer, CH_TEXT("%llu"), ulong);
#else
	sprintf(buffer, "%llu", ulong);
#endif

	write_raw(buffer, ch::strlen(buffer));
	return *this;
}

ch::Stream& ch::Stream::operator<<(s64 slong) {
	tchar buffer[64];
#if CH_UNICODE
	swprintf(buffer, CH_TEXT("%lli"), slong);
#else
	sprintf(buffer, "%lli", slong);
#endif

	write_raw(buffer, ch::strlen(buffer));
	return *this;
}

ch::Stream& ch::Stream::operator<<(f32 f) {
	tchar buffer[100];
#if CH_UNICODE
	swprintf(buffer, CH_TEXT("%f"), f);
#else
	sprintf(buffer, "%f", f);
#endif
	write_raw(buffer, ch::strlen(buffer));
	return *this;
}

ch::Stream& ch::Stream::operator<<(f64 d) {
	tchar buffer[100];
#if CH_UNICODE
	swprintf(buffer, CH_TEXT("%f"), d);
#else
	sprintf(buffer, "%f", d);
#endif

	write_raw(buffer, ch::strlen(buffer));
	return *this;
}

bool ch::load_file_into_memory(const tchar* path, File_Data* fd, ch::Allocator allocator) {
	FILE* f = fopen(path, "rb");
	if (!f) return false;

	fseek(f, 0, SEEK_END);
	const usize size = ftell(f);
	fseek(f, 0, SEEK_SET);

	u8* buffer = ch_new(allocator) u8[size];
	fread(buffer, size, 1, f);
	fclose(f);
	buffer[size] = 0;

	fd->data = buffer;
	fd->size = size;
	fd->allocator = allocator;

	return true;
}

void ch::File_Data::free() {
	if (data) {
		operator ch_delete(data, allocator);
	}
}
