#pragma once

#include <stdio.h>

// @HACK(CHall): I get it. This is bad
#define TEST_FAIL(string, ...) printf("  %i: test failed at line %i because ", test_failed + 1, __LINE__); printf(string, __VA_ARGS__); printf("\n"); test_failed += 1;
#define TEST_PASS(string, ...) printf("  "); printf(string, __VA_ARGS__); printf(" passed\n");

extern int test_failed;

void manual_string_test();