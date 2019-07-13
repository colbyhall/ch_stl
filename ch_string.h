#pragma once

#include "ch_allocator.h"
#include "ch_memory.h"

/* TODO *
 * String pool allocator instead of general heap allocator
 * UTF 8 decoder and encoder
 */

namespace ch {
	template <typename T>
	usize strlen(const T* c_str) {
		for (usize i = 0; i < U64_MAX; i++) {
			if (c_str[i] == 0) {
				return i;
			}
		}

		return 0;
	}

    const tchar eol = '\n';
    const tchar eos = 0;

    CH_FORCEINLINE bool is_whitespace(u32 c) {
        return c >= '\t' && c <= '\r' || c == ' ';
    }

	CH_FORCEINLINE bool is_letter(u32 c) {
		c |= 0x20;
		return c >= 'a' && c <= 'z';
	}

	CH_FORCEINLINE bool is_digit(u32 c) {
		return c >= '0' && c <= '9';
	}

	CH_FORCEINLINE bool is_oct_digit(u32 c) {
		return c >= '0' && c <= '7';
	}

	CH_FORCEINLINE bool is_hex_digit(u32 c) {
		if (c >= '0' && c <= '9') return true;
		if (ch::is_letter(c)) return true;

		return false;
	}

	CH_FORCEINLINE bool is_symbol(u32 c) {
		if (c >= '!' && c <= '/') return true;
		if (c >= ':' && c <= '?') return true;
		c |= 0x20;
		if (c >= '{' && c <= '~') return true;

		return false;
	}

	CH_FORCEINLINE bool is_lowercase(u32 c) {
		return c >= 'a' && c <= 'z';
	}

	CH_FORCEINLINE bool is_uppercase(u32 c) {
		return c >= 'A' && c <= 'Z';
	}

	CH_FORCEINLINE u32 to_lowercase(u32 c) {
		u32 d = c | 0x20;
		if (d >= 'a' && d <= 'z') return d;

		return c;
	}

	CH_FORCEINLINE u32 to_uppercase(u32 c) {
		u32 d = c & ~0x20;
		if (d >= 'A' && d <= 'Z') return d;

		return c;
	}

	template <typename T>
	struct Base_String {
        T* data;
        usize count;
        usize allocated;
        ch::Allocator allocator;

        Base_String(const ch::Allocator& in_alloc = ch::get_heap_allocator()) : data(nullptr), count(0), allocated(0), allocator(in_alloc) {}
		explicit Base_String(const T* in_str, const ch::Allocator& in_alloc = ch::get_heap_allocator()) {
			count = ch::strlen(in_str);
			allocated = count + 1;
			allocator = in_alloc;
			data = ch_new(allocator) T[allocated];
			ch::mem_copy(data, in_str, count * sizeof(T));
			data[count] = 0;
		}

		Base_String<T>& operator=(const T* c_str) {
			const usize c_str_count = ch::strlen(c_str);
			if (c_str_count + 1 > allocated) {
				if (data) {
					assert(allocator);
					operator ch_delete[](data, allocator);
				}
				allocated = c_str_count + 1;
				data = ch_new(allocator) tchar[allocated];
			}

			count = c_str_count;
			ch::mem_copy(data, c_str, count * sizeof(tchar));
			return *this;
		}

        explicit operator bool() const { return data && count; }

        operator const T*() const {
            return data;
        }

        operator T*() {
            return data;
        }

        T& operator[](usize index) {
            assert(index < count);
            return data[index];
        }

        const T& operator[](usize index) const {
            assert(index < count);
            return data[index];
        }

        bool operator==(const Base_String<T>& right) const {
            if (count != right.count) return false;

            for (usize i = 0; i < count; i++) {
                if (data[i] != right[i]) return false;
            }

            return true;
        }

        bool operator!=(const Base_String<T>& right) const {
            return !(*this == right);
        }

		bool operator==(const T* c_str) const {
			const usize c_count = ch::strlen(c_str);
			if (count != c_count) return false;

			for (usize i = 0; i < count; i++) {
				if (data[i] != c_str[i]) return false;
			}

			return true;
		}

		bool operator!=(const tchar* c_str) const {
			return !(*this == c_str);
		}
        
        void reserve(usize amount) {
            assert(allocator);

            allocated += amount;

            if (data) {
                data = (T*)ch::realloc(data, allocated * sizeof(T));
            } else {
                data = ch_new(allocator) T[allocated];
            }
        }

        void destroy() {
            if (data) {
                assert(allocated && allocator);
                operator ch_delete[](data, allocator);
                data = nullptr;
            }
            count = 0;
            allocated = 0;
        }

        void advance(usize amount) {
            assert(amount < allocated);
            data += amount;
            count -= amount;
            allocated -= amount;
        }

        void eat_whitespace() {
            while (count > 0 && is_whitespace(data[0])) {
                advance(1);
            }
        }

        Base_String<T> eat_line() {
            eat_whitespace();

            Base_String<T> result;
            for (usize i = 0; i < count; i++) {
                if (ch::is_eol(data[i])) {
                    result.data = data;
                    result.count = i;
                    result.allocated = i;
                    result.allocator = allocator;
                    advance(i + 1);

                    return result;
                }
            }

            return result;
        }

		CH_FORCEINLINE bool starts_with(const T* c_str, bool case_matters = false) {
			ch::Base_String<T> string;
			string.data = (T*)c_str;
			string.count = ch::strlen(c_str);
			string.allocated = string.count;

			return starts_with(string, case_matters);
		}

		CH_FORCEINLINE bool ends_with(const T* c_str, bool case_matters = false) {
			ch::Base_String<T> string;
			string.data = (T*)c_str;
			string.count = ch::strlen(c_str);
			string.allocated = string.count;

			return ends_with(string, case_matters);
		}

		bool starts_with(const Base_String<T>& in_str, bool case_matters = false) {
			if (!count || !in_str) return false;

			for (usize i = 0; i < in_str.count; i++) {
				T a = data[i];
				T b = in_str[i];

				if (b == ch::eos) return true;

				if (!case_matters && (ch::is_letter(a) && ch::is_letter(b))) {
					a = ch::to_lowercase(a);
					b = ch::to_lowercase(b);
					if (a != b) return false;
				}
				else if (a != b) return false;
			}

			return true;
		}

		bool ends_with(const Base_String<T>& in_str, bool case_matters = false) {
			if (count < in_str.count) return false;

			for (usize i = 0; i < in_str.count; i++) {
				T a = data[count - 1 - i];
				T b = in_str[in_str.count - 1 - i];

				if (!case_matters && (ch::is_letter(a) && ch::is_letter(b))) {
					a = ch::to_lowercase(a);
					b = ch::to_lowercase(b);
					if (a != b) return false;
				}
				else if (a != b) return false;
			}

			return true;			
		}
	};

	using String   = ch::Base_String<tchar>;
	using String32 = ch::Base_String<u32>;
}
