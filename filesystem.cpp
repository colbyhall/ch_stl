#include "filesystem.h"

// @HACK(CHall): We don't need to use cstl
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <stdio.h>

ch::Path::Path(const char* in_path) : count(ch::strlen(in_path)) {
	assert(ch::strlen(in_path) < ch::max_path);
	ch::mem_copy(data, in_path, count);
	data[count] = 0;
}

ch::Path::Path(const ch::String& s) {
	assert(s.count < ch::max_path);
	ch::mem_copy(data, s.data, s.count);

	count = s.count;
	data[count] = 0;
}

void ch::Path::append(const char* ap, bool ensure_proper) {
	usize ap_count = ch::strlen(ap);
	if (!ap_count) return;

	if (ensure_proper) {
		const char last = data[count - 1];
		const char ap_first = ap[0];

		if ((last == '\\' || last == '/') && (ap_first == '\\' || ap_first == '/')) {
			ap += 1;
			ap_count -= 1;
		}

		if ((last != '\\' && last != '/') && (ap_first != '\\' && ap_first != '/')) {
			data[count] = '\\';
			count += 1;
		}
	}

	assert(ap_count + count <= allocated);

	for (usize i = 0; i < ap_count; i++) {
		data[count + i] = ap[i];
	}
	count += ap_count;
	data[count] = 0;
}

void ch::Path::remove_until_directory() {
	for (usize i = count - 1; i >= 0; i--) {
		char c = data[i];
		if (c == '\\' || c == '/') {
			count = i;
			data[count] = 0;
			break;
		}
	}
}

const ch::String ch::Path::get_extension() {
	for (ssize i = count - 1; i >= 0; i--) {
		char c = data[i];
		if (c == '.' && i < (ssize)count - 2) {
			ch::String result;
			result.data = data + i + 1;
			result.count = count - i - 1;
			result.allocated = result.count;
			return result;
		}
	}

	return ch::String();
}

const ch::String ch::Path::get_filename() {
	ssize extension_loc = -1;
	for (ssize i = count - 1; i >= 0; i--) {
		char c = data[i];
		if (c == '.' && i < (ssize)count - 2) {
			extension_loc = i;
		}

		if (c == '\\' || c == '/') {
			if (extension_loc > -1) {
				ch::String result;
				result.data = data + i + 1;
				result.count = extension_loc - i - 1;
				return result;
			}
			break;
		}
	}

	ch::String result;
	result.data = data;
	result.count = extension_loc;
	return result;
}

ch::Stream& ch::Stream::operator<<(bool b) {
	*this << (b ? "true" : "false");
	return *this;
}

ch::Stream& ch::Stream::operator<<(u8 b) {
	char buffer[5];
	const usize size = ch::sprintf(buffer, "%u", b);

	write_raw(buffer, size);
	return *this;
}

ch::Stream& ch::Stream::operator<<(s8 b) {
	char buffer[5];
	const usize size = ch::sprintf(buffer, "%i", b);

	write_raw(buffer, size);
	return *this;
}

ch::Stream& ch::Stream::operator<<(u16 s) {
	char buffer[10];
	const usize size = ch::sprintf(buffer, "%u", s);

	write_raw(buffer, size);
	return *this;
}

ch::Stream& ch::Stream::operator<<(s16 s) {
	char buffer[10];
	const usize size = ch::sprintf(buffer, "%i", s);

	write_raw(buffer, size);
	return *this;
}

ch::Stream& ch::Stream::operator<<(u32 uint) {
	char buffer[32];
	const usize size = ch::sprintf(buffer, "%u", uint);

	write_raw(buffer, size);
	return *this;
}

ch::Stream& ch::Stream::operator<<(s32 i) {
	char buffer[32];
	const usize size = ch::sprintf(buffer, "%i", i);

	write_raw(buffer, size);
	return *this;
}

ch::Stream& ch::Stream::operator<<(u64 ulong) {
	char buffer[64];
	const usize size = ch::sprintf(buffer, "%llu", ulong);

	write_raw(buffer, size);
	return *this;
}

ch::Stream& ch::Stream::operator<<(s64 slong) {
	char buffer[64];
	const usize size = ch::sprintf(buffer, "%lli", slong);

	write_raw(buffer, size);
	return *this;
}

ch::Stream& ch::Stream::operator<<(f32 f) {
	char buffer[100];
	const usize size = ch::sprintf(buffer, "%f", f);
	
	write_raw(buffer, size);
	return *this;
}

ch::Stream& ch::Stream::operator<<(f64 d) {
	char buffer[100];
	const usize size = ch::sprintf(buffer, "%f", d);

	write_raw(buffer, size);
	return *this;
}

bool ch::load_file_into_memory(const char* path, File_Data* fd, ch::Allocator allocator) {
	ch::File f;
	defer(f.close());
	if (!f.open(path, ch::FO_Read | ch::FO_Binary)) return false;

	const usize fs = f.size();
	fd->allocator = allocator;
	fd->size = fs + 1;
	fd->data = ch_new(allocator) u8[fd->size];
	f.read(fd->data, fs);
	fd->data[fs] = 0;

	return true;
}

void ch::File_Data::free() {
	if (data) {
		operator ch_delete(data, allocator);
	}
}


ch::String ch::File_Data::to_string() const {
	ch::String result;
	result.reserve(size);
	result.count = size;
	for (usize i = 0; i < size; i++) {
		result[i] = data[i];
	}

	return result;
}

ch::Recursive_Directory_Iterator::Recursive_Directory_Iterator(const ch::Path& path) {
	current_path = path;
	Directory_Iterator iter(path);
	iterators.push(iter);
}

ch::Recursive_Directory_Iterator::~Recursive_Directory_Iterator() {
	iterators.free();
}

void ch::Recursive_Directory_Iterator::advance() {
	ch::Directory_Iterator& current = iterators.back();
	current.advance();

	while (iterators.count > 1 && !current.can_advance()) {
		iterators.pop();
		current = iterators.back();
		current_path.remove_until_directory();
		current.advance();
	}

	if (current.can_advance()) {
		ch::Directory_Result r = current.get();
		if (r.type == DRT_Directory && r.file_name[0] != '.') {
			current_path.append("\\");
			current_path.append(r.file_name);
			ch::Directory_Iterator iter(current_path);
			iterators.push(iter);
		}
	}
}

bool ch::Recursive_Directory_Iterator::can_advance() const {
	if (!iterators) return false;

	return iterators.back().can_advance();
}

ch::Directory_Result ch::Recursive_Directory_Iterator::get() {
	return iterators.back().get();
}
