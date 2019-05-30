#pragma once

#include "ch_allocator.h"
#include "ch_memory.h"

#include <string>

namespace ch {

	constexpr usize strlen(const char* c_str);

	template <typename T>
	struct TString {
		T* data;
		usize count;
		usize allocated;
		Allocator allocator;

		TString() {
			count = 0;
			allocated = 0;
			allocator = get_heap_allocator();
			data = nullptr;
		}

		TString(const char* c_str) {
			allocator = get_heap_allocator();

			count = strlen(c_str);
			allocated = count;
			data = ch_new(allocator) T[count];
			ch::memcpy(data, c_str, count);
		}

		TString(const TString<T>& copy) {
			count = copy.count;
			allocated = copy.allocated;
			allocator = copy.allocator;
			data = ch_new(allocator) T[count];
			ch::memcpy(data, copy.data, count);
		}

		~TString() {
			if (data) {
				operator ch_delete(data, allocator);
				data = nullptr;
			}
		}

		explicit operator bool() const {
			return data && count;
		}
	};

	using  String = TString<u8>;
	using WString = TString<s16>;
}
