#include "filesystem.h"

// @HACK(CHall): We don't need to use cstl
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <stdio.h>

ch::Path::Path(const tchar* in_path) : count(ch::strlen(in_path)) {
	assert(ch::strlen(in_path) < ch::max_path);
	ch::mem_copy(data, in_path, count * sizeof(tchar));
	data[count] = 0;
}

void ch::Path::append(const tchar* ap) {
	usize ap_count = ch::strlen(ap);
	if (!ap_count) return;

	const tchar last = data[count - 1];
	const tchar ap_last = ap[ap_count - 1];
	if ((last == '\\' || last == '/') && (ap_last == '\\' || ap_last == '/')) ap_count -= 1;

	assert(ap_count + count <= allocated);

	for (usize i = 0; i < ap_count; i++) {
		data[count + i] = ap[i];
	}
	count += ap_count;
	data[count] = 0;
}

void ch::Path::remove_until_directory() {
	for (usize i = count - 1; i >= 0; i--) {
		tchar c = data[i];
		if (c == '\\' || c == '/') {
			count = i;
			data[count] = 0;
			break;
		}
	}
}

ch::String ch::Path::get_extension() {
	for (usize i = count - 1; i >= 0; i--) {
		tchar c = data[i];
		if (c == '.') {
			ch::String result;
			result.data = data + i;
			result.count = count - i;
			result.allocated = result.count;
			return result;
		}
	}

	return ch::String();
}

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


ch::Recursive_Directory_Iterator::Recursive_Directory_Iterator(const ch::Path& path) {
	current_path = path;
	Directory_Iterator iter(path);
	current_iterator = iterators.push(iter);
}

ch::Recursive_Directory_Iterator::~Recursive_Directory_Iterator() {
	iterators.free();
}

void ch::Recursive_Directory_Iterator::advance() {
	if (current_iterator + 1 < iterators.count) {
		current_iterator += 1;
		return;
	}

	ch::Directory_Iterator& current = iterators[current_iterator];
	current.advance();

	while (!current.can_advance()) {
		if (current_iterator == 0) break;
		current_iterator -= 1;
		iterators.pop();
		current = iterators[current_iterator];
		current_path.remove_until_directory();
		current.advance();
	}

	if (current.can_advance()) {
		ch::Directory_Result r = current.get();
		if (r.type == DRT_Directory && r.file_name[0] != '.') {
			current_path.append(CH_TEXT("\\"));
			current_path.append(r.file_name);
			ch::Directory_Iterator iter(current_path);
			iterators.push(iter);
		}
	}
}

bool ch::Recursive_Directory_Iterator::can_advance() const {
	if (!iterators) return false;

	return iterators[current_iterator].can_advance();

	return false;
}

ch::Directory_Result ch::Recursive_Directory_Iterator::get() {
	return iterators[current_iterator].get();
}