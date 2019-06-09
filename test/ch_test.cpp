/**
 * ch_stl lib test
 * used to ensure code is working
 * currently under construction
*/

#include "ch_test.h"

#include <ch_allocator.h>
#include <ch_memory.h>
#include <ch_array.h>
#include <ch_string.h>
#include <ch_templates.h>
#include <ch_filesystem.h>
#include <ch_math.h>
#include <ch_window.h>

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
}

static void string_test() {
    ch::String foo = TEXT("hello world");
    ch::String bar = foo;

    if (foo.data != bar.data) {
        TEST_FAIL("String has a reallocation");
    } else {
        TEST_PASS("String copy")
    }
}

static void math_test() {
    ch::Vector2 vec = 5.f;
    ch::Vector2 vec_n = vec.get_normalized();
    if (!vec_n.is_normalized()) {
        TEST_FAIL("Vec2 normalized failed");
    } else {
        TEST_PASS("Vec2 nomalized");
    }
}

bool exit_requested = false;

static void window_test() {
    ch::Window window;
    ch::create_window(TEXT("whbat the fuck"), 512, 512, 0, &window);
    window.set_visibility(true);

    auto exit_request = [](const ch::Window& window) {
        exit_requested = true;
    };

    window.on_exit_requested = exit_request;

    while(!exit_requested) {
        ch::poll_events();
    }
}

int main() {
    printf("-------Beginning c_stl Test-------\n");
    memory_test();
    array_test();
	string_test();
    math_test();
    window_test();
    printf("-------c_stl Test Finished-------\n");

    if (test_failed) {
        printf("%i test failed\n\nPlease report to author(s)\n", test_failed);
    }

    for(;;) {}
}