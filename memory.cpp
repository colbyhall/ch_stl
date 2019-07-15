#include "memory.h"
#include "allocator.h"

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