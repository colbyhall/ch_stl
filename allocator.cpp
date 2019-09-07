#include "allocator.h"
#include "memory.h"

void* ch::Allocator::alloc(usize size) {
    return func(*this, nullptr, size);
}

void* ch::Allocator::realloc(void* ptr, usize size) {
    return func(*this, ptr, size);
}

void ch::Allocator::free(void* ptr) {
    func(*this, ptr, 0);
}

static void* heap_alloc(const ch::Allocator& allocator, void* ptr, usize size) {
    void* result = nullptr;
    if (size) {
        if (!ptr) {
            result = ch_malloc(size);
        } else {
            result = ch_realloc(ptr, size);
        }
    } else if (ptr) {
        ch_free(ptr);
    }

    return result;
}

// Use static initializer to avoid the static initialization order fiasco
ch::Allocator ch::context_allocator = { nullptr, heap_alloc };

ch::Allocator ch::get_heap_allocator() {
    ch::Allocator result;
    result.data = nullptr;
    result.func = heap_alloc;
    return result;
}

static void* arena_alloc(const ch::Allocator& allocator, void* ptr, usize size) {
    ch::Arena_Allocator_Header* header = allocator.get_header<ch::Arena_Allocator_Header>();

    if (!ptr || (ptr && size > 0)) {
        // @NOTE(Colby): this will assert if we're trying to free with a nullptr
        assert(size > 0);
        assert(header->current + size <= header->allocated);
        u8* result = (u8*)header->data + header->current;

		if (ptr) {
			ch::mem_copy(result, ptr, size);
		}

        header->current += size;
        return result;
    } 

	return nullptr;
}

ch::Allocator ch::make_arena_allocator(usize size) {
    u8* data = (u8*)ch_malloc(size + sizeof(ch::Arena_Allocator_Header));
    ch::Arena_Allocator_Header* header = (ch::Arena_Allocator_Header*)data;
    *header = {};
    header->data = data + sizeof(ch::Arena_Allocator_Header);
    header->allocated = size;

    return { data, arena_alloc };
}

void ch::free_arena_allocator(ch::Allocator* allocator) {
    ch::Arena_Allocator_Header* header = allocator->get_header<ch::Arena_Allocator_Header>();
    ch_free(allocator->data);
    allocator->data = nullptr;
}

void ch::reset_arena_allocator(ch::Allocator* allocator) {
    ch::Arena_Allocator_Header* header = allocator->get_header<ch::Arena_Allocator_Header>();
    header->current = 0;
}

void* operator new(usize size, ch::Allocator allocator) {
    return allocator.alloc(size);
}

void* operator new[](usize size, ch::Allocator allocator) {
    return allocator.alloc(size);
}

void operator delete(void* ptr, ch::Allocator allocator) {
    return allocator.free(ptr);
}

void operator delete[](void* ptr, ch::Allocator allocator) {
    return allocator.free(ptr);
}