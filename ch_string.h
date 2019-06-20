#pragma once

#include "ch_allocator.h"
#include "ch_memory.h"
#include "ch_assert.h"

/* TODO *
 * String pool allocator instead of general heap allocator
 * UTF 8 decoder and encoder
 * u32 String
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

    const tchar endl = '\n';
    const tchar eol = ch::endl;
    const tchar eof = 0;

    inline bool is_eol(u32 c) {
        return c == ch::endl || c == '\r' || c == ch::eof;
    }

    inline bool is_whitespace(u32 c) {
        return c >= '\t' && c <= '\r' || c == ' ';
    }

    /**
     * Base String
     *
     * No copy constructor for a reason
     */
	template <typename T>
	struct Base_String {
        T* data;
        usize count;
        usize allocated;
        ch::Allocator allocator;

        Base_String(const ch::Allocator& in_alloc = ch::get_heap_allocator()) : data(nullptr), count(0), allocated(0), allocator(in_alloc) {}
		explicit Base_String(const T* in_str, const ch::Allocator& in_alloc = ch::get_heap_allocator()) {
			count = ch::strlen<T>(in_str);
			allocated = count + 1;
			allocator = in_alloc;
			data = ch_new(allocator) T[allocated];
			ch::memcpy(data, in_str, count * sizeof(T));
			data[count] = 0;
		}

		Base_String<T>& operator=(const T* c_str) {
			const usize c_str_count = ch::strlen<T>(c_str);
			if (c_str_count + 1 > allocated) {
				if (data) {
					assert(allocator);
					operator ch_delete[](data, allocator);
				}
				allocated = c_str_count + 1;
				data = ch_new(allocator) tchar[allocated];
			}

			count = c_str_count;
			ch::memcpy(data, c_str, count * sizeof(tchar));
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
            return data[count];
        }

        const T& operator[](usize index) const {
            assert(index < count);
            return data[count]
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
			const usize c_count = ch::strlen<T>(c_str);
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
	};

	using String   = ch::Base_String<tchar>;
	using String32 = ch::Base_String<u32>;
}
