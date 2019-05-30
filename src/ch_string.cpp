#include "ch_string.h"

constexpr usize ch::strlen(const char* c_str) {
	for (usize i = 0; i < U64_MAX; i++) {
		if (c_str[i] == 0) {
			return i;
		}
	}

	return 0;
}
