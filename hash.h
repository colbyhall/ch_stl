#pragma once

#include "types.h"

namespace ch {
	using Hash_Function = u64(*)(const void* s, usize count);

	const u64 FNV_offset_basic = 0xcbf29ce484222325;
	const u64 FNV_prime = 0x100000001b3;

	CH_FORCEINLINE u64 fnv1_hash(const void* s, usize count) {
		u64 hash = ch::FNV_offset_basic;
		u8* casted_s = (u8*)s;
		for (usize i = 0; i < count; i++) {
			hash *= ch::FNV_prime;
			hash = hash ^ casted_s[i];
		}

		return hash;
	}
}