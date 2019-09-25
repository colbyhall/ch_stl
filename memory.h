#pragma once

#include "types.h"

namespace ch {
    void* malloc(usize size);
    void* realloc(void* ptr, usize size);
    void free(void* ptr);

#if CH_BUILD_DEBUG
	void* debug_malloc(usize size, const char* file, u64 line);
	void* debug_realloc(void* ptr, usize size, const char* file, u64 line);
	void debug_free(void* ptr);

	usize get_total_allocated();
	usize get_num_allocations();
#endif

    void mem_copy(void* dest, const void* src, usize size);
    void* mem_move(void* dest, const void* src, usize size);
    
    CH_FORCEINLINE void mem_set(void* ptr, usize size, u8 c) {
        u8* casted_ptr = (u8*)ptr;

        for (usize i = 0; i < size; i++) {
            casted_ptr[i] = c;
        }
    }

    CH_FORCEINLINE void mem_zero(void* ptr, usize size) {
        mem_set(ptr, size, 0);
    }
}

#define ch_new new
#define ch_delete delete

#if CH_BUILD_DEBUG
#define ch_malloc(size) ch::debug_malloc(size, __FILE__, __LINE__)
#define ch_realloc(ptr, size) ch::debug_realloc(ptr, size, __FILE__, __LINE__)
#define ch_free(ptr) ch::debug_free(ptr)
#else
#define ch_malloc(size) ch::malloc(size)
#define ch_realloc(ptr, size) ch::realloc(ptr, size)
#define ch_free(ptr) ch::free(ptr);
#endif

void* operator new(usize size);
void* operator new[](usize size);

void operator delete(void* ptr);
void operator delete[](void* ptr);