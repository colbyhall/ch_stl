#pragma once

#include "ch_types.h"

namespace ch {
    void* malloc(usize size);
    void* realloc(void* ptr, usize size);
    void free(void* ptr);

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