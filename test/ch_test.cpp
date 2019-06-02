/**
 * ch_stl lib test
 * used to ensure code is working
 * currently under construction
*/

#include "ch_test.h"

#include <ch_allocator.h>
#include <ch_memory.h>
#include <ch_array.h>
#define CH_STRING_AUTO_MEMORY
#include <ch_string.h>
#include <ch_templates.h>

int test_failed = 0;

static void memory_test() {
    const usize buffer_size = 255;
    u8* buffer = ch_new u8[buffer_size];
    // defer(ch_delete[] buffer);

    const u8 set_value = 0;
    ch::memset(buffer, buffer_size, set_value);
    for (usize i = 0; i < buffer_size; i++) {
        if (buffer[i] != set_value) {
            TEST_FAIL("ch::memset did not set all values in buffer %i", set_value);
            break;
        }

        buffer[i] = (u8)i;
    }

    // @NOTE(CHall): Is this a good test?
    ch::memmove(buffer, buffer + 5, 7);
    if (buffer[0] != 5) {
        TEST_FAIL("ch::memmove is broken");
    }
}

static void array_test() {
    {
        ch::Array<float> array;
        array.push(-25.f);
        if (array.count != 1 || array[0] != -25.f) {
            TEST_FAIL("Array<float>::add is failing");
        } else {
            TEST_PASS("Array<float>::add");
        }
        array.push(45.f);
        array.push(20.f);
        array.remove(1);
        if (array[1] != 20.f) {
            TEST_FAIL("Array<float>::remove is failing to remove index 1");
        } else {
            TEST_PASS("Array<float>::remove");
        }
    }

    {
        ch::Array<float> array = { 120.f, 12.f, 5.f };
        if (array[0] != 120.f || array[1] != 12.f || array[2] != 5.f) {
            TEST_FAIL("Array<float> initializer list construction failed");
        } else {
            TEST_PASS("Array<float> initializer list");
        }
    }

    {
        ch::Array<int> foo = { 123, 123, 123, 123 };
        ch::Array<int> bar(foo);

        if (foo != bar) {
            TEST_FAIL("Array copy constuctor is broken");
        } else {
            TEST_PASS("Array copy constructor");
        }

        foo.reserve(1024);
        bar = foo;
        if (foo != bar) {
            TEST_FAIL("Array copy assignment is broken");
        } else {
            TEST_PASS("Array copy assignemnt");
        }
    }

    {
        ch::Array<int> foo = { 123, 123, 123, 123 };
        ch::Array<int> bar(ch::move(foo));

        if (foo || !bar) {
            TEST_FAIL("Array move constuctor is broken");
        } else {
            TEST_PASS("Array move constructor");
        }

        foo.reserve(1024);
        bar = ch::move(foo);
        if (foo || !bar) {
            TEST_FAIL("Array move assignment is broken");
        } else {
            TEST_PASS("Array move assignment");
        }
    }
}

static void auto_string_test() {
    ch::String foo = "hello world";
    ch::String bar(ch::move(foo));

    {
        ch::String foo = "hello world";
        ch::String bar(foo);

        if (foo != bar) {
            TEST_FAIL("String copy constuctor is broken");
        } else {
            TEST_PASS("String copy constructor");
        }

        foo = "what the fuck jerry";
        bar = foo;
        if (foo != bar) {
            TEST_FAIL("String copy assignment is broken");
        } else {
            TEST_PASS("String copy assignment");
        }
    }

    {
        ch::String foo = "hello world";
        ch::String bar(ch::move(foo));

        if (foo || !bar) {
            TEST_FAIL("String move constuctor is broken");
        } else {
            TEST_PASS("String move constuctor");
        }

        foo = "what the fuck jerry";
        bar = ch::move(foo);
        if (foo || !bar) {
            TEST_FAIL("String move assignment is broken");
        } else {
            TEST_PASS("String move assignment");
        }
    }
}

int main() {
    printf("-------Beginning c_stl Test-------\n");
    memory_test();
    array_test();
	auto_string_test();
    manual_string_test();
    printf("-------c_stl Test Finished-------\n");

    if (test_failed) {
        printf("%i test failed\n\nPlease report to author(s)\n", test_failed);
    } 

    for(;;) {}
}