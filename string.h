#pragma once

#include "allocator.h"
#include "memory.h"
#include "hash.h"

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

	template <typename T>
	bool streq(const T* a, const T* b) {
		for (usize i = 0; i < U64_MAX; i++) {
			const char a_c = a[i];
			const char b_c = b[i];

			if (a_c != b_c) return false;
			if (a_c == 0 || b_c == 0) {
				if (a_c == b_c) return true;
				return false;
			}
		}

		return true;
	}

    const char eol = '\n';
    const char eos = 0;

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

        Base_String(const ch::Allocator& in_alloc = ch::context_allocator) : data(nullptr), count(0), allocated(0), allocator(in_alloc) {}
		explicit Base_String(const T* in_str, const ch::Allocator& in_alloc = ch::context_allocator) {
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
				data = ch_new(allocator) char[allocated];
			}

			count = c_str_count;
			ch::mem_copy(data, c_str, count * sizeof(char));
			return *this;
		}

        explicit operator bool() const { return data && count > 0; }

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

		bool operator!=(const char* c_str) const {
			return !(*this == c_str);
		}
        
        void reserve(usize amount) {
            assert(allocator);

            allocated += amount;

            if (data) {
                data = (T*)ch_realloc(data, allocated * sizeof(T));
            } else {
                data = ch_new(allocator) T[allocated];
            }
        }

        void free() {
            if (data) {
                assert(allocated && allocator);
                operator ch_delete[](data, allocator);
                data = nullptr;
            }
            count = 0;
            allocated = 0;
        }

		ch::Base_String<T> copy(const ch::Allocator& in_alloc = ch::context_allocator) const  {
			ch::Base_String<T> result;
			result.allocator = in_alloc;
			result.reserve(count);
			ch::mem_copy(result.data, data, count * sizeof(T));
			result.count = count;
			return result;
		}

		CH_FORCEINLINE T* to_tstring(ch::Allocator& in_alloc = ch::context_allocator) const {
			T* r = (T*)in_alloc.alloc((count + 1) * sizeof(T));
			for (usize i = 0; i < count; i += 1) {
				r[i] = data[i];
			}
			r[count] = 0;

			return r;
		}

        void advance(usize amount) {
            assert(amount <= count);
            data += amount;
            count -= amount;
            allocated -= amount;
        }

        u32 eat_whitespace() {
			const usize old_count = count;
            while (count > 0 && is_whitespace(data[0])) {
                advance(1);
            }
			return (u32)(old_count - count); 
        }

        Base_String<T> eat_line() {
            Base_String<T> result;
            for (usize i = 0; i < count; i++) {
                if (data[i] == ch::eol) {
                    result.data = data;
                    result.count = i;
                    result.allocated = i;
                    result.allocator = allocator;
                    advance(i + 1);

					if (result.count > 2 && result[i - 1] == '\r') {
						result.count -= 1;
					}

                    return result;
				} else if (data[i] == 0) {
					result.data = data;
					result.count = i;
					result.allocated = i;
					result.allocator = allocator;
					
					count = 0;
					return result;
				}
            }

			result = *this;
			count = 0;

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

		void append(const char* ap) {
			const usize ap_size = ch::strlen(ap);
			if (count + ap_size > allocated) {
				reserve(count + ap_size - allocated);
			}
			for (usize i = 0; i < ap_size; i++) {
				data[i + count] = ap[i];
			}
			count += ap_size;
		}

		ssize find_from_left(char c) const {
			for (usize i = 0; i < count; i++) {
				if (data[i] == c) return i;
			}

			return -1;
		}

		ssize find_from_right(char c) const {
			for (usize i = count - 1; i >= 0; i--) {
				if (data[i] == c) {
					return i;
				}
			}

			return -1;
		}

		void to_lowercase() {
			for (usize i = 0; i < count; i++) {
				const T c = data[i];
				if (ch::is_letter(c)) data[i] = ch::to_lowercase(c);
			}
		}

		void to_uppercase() {
			for (usize i = 0; i < count; i++) {
				const T c = data[i];
				if (ch::is_letter(c)) data[i] = ch::to_uppercase(c);
			}
		}
	};

	using String   = ch::Base_String<char>;
	using String8  = ch::Base_String<u8>;
	using String16 = ch::Base_String<u16>;
	using String32 = ch::Base_String<u32>;

	usize sprintf(char* buffer, const char* fmt, ...);
	void bytes_to_string(usize bytes, ch::String* out_string);

	bool atof(const char* tstr, f32* f);
	CH_FORCEINLINE bool atof(const ch::String& s, f32* f) {
		char* tstr = s.to_tstring();
		defer(ch::context_allocator.free(tstr));
		return atof(tstr, f);
	}

	bool atoi(const char* tstr, s32* i);
	CH_FORCEINLINE bool atoi(const ch::String& s, s32* i) {
		char* tstr = s.to_tstring();
		defer(ch::context_allocator.free(tstr));
		return atoi(tstr, i);
	}
}

template<typename T>
u64 hash(const ch::Base_String<T>& s) {
	return ch::fnv1_hash(s.data, s.count * sizeof(T));
}

CH_FORCEINLINE u64 hash(const char* c_str) {
	return ch::fnv1_hash(c_str, ch::strlen(c_str));
}
