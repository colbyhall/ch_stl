#pragma once

#include "ch_types.h"

namespace ch {
    void* malloc(usize size);
    void* realloc(void* ptr, usize size);
    void free(void* ptr);

    void memcpy(void* dest, const void* src, usize size);
    void* memmove(void* dest, const void* src, usize size);
    
    CH_FORCEINLINE void memset(void* ptr, usize size, u8 c) {
        u8* casted_ptr = (u8*)ptr;

        for (usize i = 0; i < size; i++) {
            casted_ptr[i] = c;
        }
    }

    CH_FORCEINLINE void memzero(void* ptr, usize size) {
        memset(ptr, size, 0);
    }
}