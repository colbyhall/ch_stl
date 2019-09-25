#include "memory.h"
#include "allocator.h"

#if CH_BUILD_DEBUG
struct Allocation_Header {
	void* allocation;
	usize size;

	const char* file;
	u64 line;

	Allocation_Header* parent;
	Allocation_Header* child;
};

Allocation_Header* first_header = nullptr;
usize total_allocated = 0;
usize num_allocations = 0;

void* ch::debug_malloc(usize size, const char* file, u64 line) {
	const usize orig_size = size;
	size += sizeof(Allocation_Header);
	u8* result = (u8*)ch::malloc(size);

	Allocation_Header* header = (Allocation_Header*)result;
	result += sizeof(Allocation_Header);

	*header = {};
	header->allocation = result;
	header->size = orig_size;
	header->file = file;
	header->line = line;

#if 0
	if (!first_header) {
		first_header = header;
	} else {
		Allocation_Header* current = first_header;
		while(current->child) {
			current = current->child;
		}

		current->child = header;
		header->parent = current;
	}
#endif

	total_allocated += size;
	num_allocations += 1;

	return result;
}

void* ch::debug_realloc(void* ptr, usize size, const char* file, u64 line) {
	if (!ptr) return ch::debug_malloc(size, file, line);

	Allocation_Header* old_header = (Allocation_Header*)((u8*)ptr - sizeof(Allocation_Header));
	const usize old_size = old_header->size + sizeof(Allocation_Header);
	const usize allocation_size = size + sizeof(Allocation_Header);
	u8* result = (u8*)ch::realloc(old_header, allocation_size);

	Allocation_Header* header = (Allocation_Header*)result;
	result += sizeof(Allocation_Header);

	*header = {};
	header->allocation = result;
	header->size = size;
	header->file = file;
	header->line = line;

#if 0
	Allocation_Header *child = header->child;
	Allocation_Header *parent = header->parent;
	if (child) {
		assert(child->parent == old_header);
		child->parent = header;
	}
	
	if (parent) {
		assert(parent->child == old_header);
		parent->child = header;
	} else {
		assert(old_header == first_header);
		first_header = header;
	}
#endif

	total_allocated += allocation_size - old_size;
	return result;
}

void ch::debug_free(void* ptr) {
	if (!ptr) return;

	Allocation_Header* header = (Allocation_Header*)((u8*)ptr - sizeof(Allocation_Header));

	total_allocated -= header->size + sizeof(Allocation_Header);
	num_allocations -= 1;

#if 0
	Allocation_Header* child = header->child;
	Allocation_Header* parent = header->parent;
	if (child) {
		assert(child->parent == header);
		if (parent) {
			assert(first_header != header);
			parent->child = child;
			child->parent = parent;
		} else {
			assert(first_header == header);
			first_header = child;
			child->parent = nullptr;
		}
	} else if (parent) {
		parent->child = nullptr;
	} else {
		assert(first_header == header);
		first_header = nullptr;
	}
#endif

	ch::free(header);
}

usize ch::get_total_allocated() {
	return total_allocated;
}

usize ch::get_num_allocations() {
	return num_allocations;
}

#endif

void ch::mem_copy(void* dest, const void* src, usize size) {
    const u8* casted_src = (u8*)src;
    u8* casted_dest = (u8*)dest;

    for (usize i = 0; i < size; i++) {
        casted_dest[i] = casted_src[i];
    }
}

void* ch::mem_move(void* dest, const void* src, usize size) {
    if (dest == src) return dest;

    u8* buffer = ch_new u8[size];
    defer(ch_delete(buffer));

    ch::mem_copy(buffer, src, size);
    ch::mem_copy(dest, buffer, size);

    return dest;
}

void* operator new(usize size) {
	return ch_malloc(size);
}

void* operator new[](usize size) {
	return ch_malloc(size);
}

void operator delete(void* ptr) {
	ch_free(ptr);
}

void operator delete[](void* ptr) {
	ch_free(ptr);
}