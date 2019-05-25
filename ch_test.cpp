/**
 * ch_stl lib test
 * used to ensure code is working
 * currently under construction


*/

#define CH_IMPLEMENTATION
#include "ch_stl.h"

#include <stdio.h>

int test_failed = 0;

// @HACK(CHall): I get it. This is bad
#define TEST_FAIL(string, ...) printf("ch_test failed at line %i because ", __LINE__); printf(string, __VA_ARGS__); printf("\n"); test_failed += 1;

static void memory_test() {
    const usize buffer_size = 255;
    u8* buffer = ch_new u8[buffer_size];
    defer(ch_delete[] buffer);

    const u8 set_value = 0;
    Memory::set(buffer, buffer_size, set_value);
    for (usize i = 0; i < buffer_size; i++) {
        if (buffer[i] != set_value) {
            TEST_FAIL("Memory::set did not set all values in buffer %i", set_value);
            break;
        }

        buffer[i] = (u8)i;
    }

    // @NOTE(CHall): Is this a good test?
    Memory::move(buffer, buffer + 5, 7);
    if (buffer[0] != 5) {
        TEST_FAIL("Memory::move is broken");
    }
}

static void array_test() {
    Array<float> array;
    array.add(-25.f);
    if (array.count != 1 || array[0] != -25.f) {
        TEST_FAIL("Array<float::add is failing");
    }
    array.add(45.f);
    array.add(20.f);
    array.remove_index(1);
    if (array[1] != 20.f) {
        TEST_FAIL("Array<float>::remove is failing to remove index 1");
    }
}

int main() {
    printf("-------Beginning c_stl Test-------\n");
    memory_test();
    array_test();
    printf("-------c_stl Test Finished-------\n");

    if (test_failed) {
        printf("%i test failed\n\nPlease report to author(s)\n", test_failed);
    } 
}