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
        }
        else {
            result = ch::realloc(ptr, size);
        }
    }
    else if (ptr) {
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