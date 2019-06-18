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
#include <ch_opengl.h>
#include <ch_defer.h>

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


    ch::Allocator arena = ch::make_arena_allocator(1024);
    defer(ch::destroy_arena_allocator(&arena));
    {
        u8* foo = (u8*)arena.alloc(256);
        u8* bar = (u8*)arena.alloc(256);
        ch::Arena_Allocator_Header* header = arena.get_header<ch::Arena_Allocator_Header>();
        if (header->current != 512) {
            TEST_FAIL("Arean allocator book keeping is off");
        } else {
            TEST_PASS("Arena allocator book keeping");
        }

        const usize distance = bar - foo;
        if (distance != 256) {
            TEST_FAIL("Arean allocator allocation distance makes no sense");
        } else {
            TEST_PASS("Arena allocator allocation");
        }

        ch::reset_arena_allocator(&arena);
        bar = (u8*)arena.alloc(256);
        if (header->current != 256) {
            TEST_FAIL("Arean allocator book keeping after reset is broken");
        }
        else {
            TEST_PASS("Arena allocator book keeping after reset");
        }

        if (bar != foo) {
            TEST_FAIL("Arena allocation after reset is not correct");
        } else {
            TEST_PASS("Arena allocation after reset");
        }
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

const u32 width = 1280;
const u32 height = (u32)((f32)width * (9.f / 16.f));

static void window_test() {
    ch::Window window;
    if (!ch::create_window(CH_TEXT("test window"), width, height, 0, &window)) {
        TEST_FAIL("Failed to create window");
        return;
    } else {
        TEST_PASS("Create Window");
    }
    defer(window.destroy());

    window.set_visibility(true);

    if (!window.is_visible) {
        TEST_FAIL("Failed to set window visibility");
    } else {
        TEST_PASS("Setting visibility");
    }
}

static bool exit_requested = false;

static void gl_test() {
    if (!ch::load_gl()) {
        TEST_FAIL("Failed to load opengl");
        return;
    } else {
        TEST_PASS("Loading opengl");
    }

    ch::Window window;
    if (!ch::create_gl_window(CH_TEXT("opengl test window"), width, height, 0, &window)) {
        TEST_FAIL("Failed to open a gl window");
        return;
    } else {
        TEST_PASS("Create GL Window");
    }
    defer(window.destroy());

    window.set_visibility(true);

    if (!ch::make_current(window)) {
        TEST_FAIL("Failed to set current gl window");
    } else {
        TEST_PASS("Set current gl window");
    }

    auto exit_request = [](const ch::Window& window) {
        exit_requested = true;
    };

    window.on_exit_requested = exit_request;

    while (!exit_requested) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.f, 1.f, 1.f, 1.f);

        if (!ch::swap_buffers(window)) {
            TEST_FAIL("failed to swap window buffer");
            break;
        }
        ch::poll_events();
    }

    TEST_PASS("Swap window buffer");
}

static void fs_test() {
    ch::String current_path = ch::get_current_path();
    if (!current_path) {
        TEST_FAIL("failed to get current path");
    } else {
        TEST_PASS("get current path");
        ch::std_out << current_path << ch::endl;
    }
    defer(current_path.destroy());
}

int main() {
    printf("-------Beginning c_stl Test-------\n");
    memory_test();
    array_test();
	string_test();
    math_test();
    window_test();
    gl_test();
    fs_test();
    printf("-------c_stl Test Finished-------\n");

    if (test_failed) {
        printf("%i test failed\n\nPlease report to author(s)\n", test_failed);
    }

    for(;;) {}
}