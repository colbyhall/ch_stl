#pragma once

#include "ch_types.h"

namespace ch {
    void* malloc(usize size);
    void* realloc(void* ptr, usize size);
    void free(void* ptr);

    void memcpy(void* dest, const void* src, usize size);
    void memset(void* ptr, usize size, u8 c);
    void memzero(void* ptr, usize size);
    void* memmove(void* dest, const void* src, usize size);
}