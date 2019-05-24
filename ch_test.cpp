/**
 * ch_stl lib test
 * used to ensure code is working
 * currently under construction


*/

#define CH_IMPLEMENTATION
#include "ch_stl.h"

#include <stdio.h>

// @HACK(CHall): I get it. This is bad
#define TEST_FAIL(string, ...) printf("ch_test failed at line %i because ", __LINE__); printf(string, __VA_ARGS__); printf("\n")

static void memory_test() {
    const usize buffer_size = 1024;
    u8* buffer = ch_new u8[buffer_size];

    const u8 set_value = 0;
    Memory::set(buffer, buffer_size, set_value);
    for (usize i = 0; i < buffer_size; i++) {
        if (buffer[i] == set_value) {
            TEST_FAIL("Memory::set did not set all values in buffer %i", set_value);
            break;
        }
    }

    defer(ch_delete[] buffer);
}

static void array_test() {

}

int main() {
    memory_test();
    array_test();
}