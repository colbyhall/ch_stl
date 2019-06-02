#include "ch_test.h"

#include <ch_string.h>

// @NOTE(CHall): We had to throw this into a different translation unit

void manual_string_test() {
    ch::String foo = "    hello world";
    ch::String bar = foo;

    if (foo.data != bar.data) {
        TEST_FAIL("Manual String copy assignment is broken");
    } else {
        TEST_PASS("Manual String copy assignment");
    }

    bar.eat_whitespace();
    if (ch::is_whitespace(bar[0])) {
        TEST_FAIL("Manual String eat_whitespace is broken");
    } else {
        TEST_PASS("Manual String eat_whitespace");
    }

}