#include "ch_allocator.h"
#include "ch_memory.h"

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
            result = ch::malloc(size);
        } else {
            result = ch::realloc(ptr, size);
        }
    } else if (ptr) {
        ch::free(ptr);
    }

    return result;
}

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
        void* result = (u8*)header->data + header->current;
        header->current += size;
        return result;
    } 

	return nullptr;
}

ch::Allocator ch::make_arena_allocator(usize size) {
    u8* data = (u8*)ch::malloc(size + sizeof(ch::Arena_Allocator_Header));
    ch::Arena_Allocator_Header* header = (ch::Arena_Allocator_Header*)data;
    *header = {};
    header->data = data + sizeof(ch::Arena_Allocator_Header);
    header->allocated = size;

    return { data, arena_alloc };
}

void ch::destroy_arena_allocator(ch::Allocator* allocator) {
    ch::Arena_Allocator_Header* header = allocator->get_header<ch::Arena_Allocator_Header>();
    ch::free(allocator->data);
    allocator->data = nullptr;
}

void ch::reset_arena_allocator(ch::Allocator* allocator) {
    ch::Arena_Allocator_Header* header = allocator->get_header<ch::Arena_Allocator_Header>();
    header->current = 0;
}

void* operator new(usize size) {
    return ch::malloc(size);
}

void* operator new[](usize size) {
    return ch::malloc(size);
}

void operator delete(void* ptr) {
    ch::free(ptr);
}

void operator delete[](void* ptr) {
    ch::free(ptr);
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