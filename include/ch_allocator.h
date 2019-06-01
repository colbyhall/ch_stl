#pragma once

#include "ch_types.h"

namespace ch {
    struct Allocator;

    using Allocator_Func = void* (*)(const Allocator& allocator, void* ptr, usize size);

    struct Allocator {
        u8* data;
        Allocator_Func func;

        explicit operator bool() const {
            return func != nullptr;
        }

        void* alloc(usize size);
        void* realloc(void* ptr, usize size);
        void free(void* ptr);

        template <typename T> 
        T* get_header() const {
            return (T*)data;
        }
    };

    Allocator get_heap_allocator();
}

#define ch_new new
#define ch_delete delete

void* operator new(usize size);
void* operator new[](usize size);

void operator delete(void* ptr);
void operator delete[](void* ptr);

void* operator new(usize size, ch::Allocator allocator);
void* operator new[](usize size, ch::Allocator allocator);

void operator delete(void* ptr, ch::Allocator allocator);
void operator delete[](void* ptr, ch::Allocator allocator);