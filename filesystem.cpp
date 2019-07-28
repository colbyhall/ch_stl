#include "filesystem.h"

// @HACK(CHall): We don't need to use cstl
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <stdio.h>

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
