#pragma once

#include "ch_allocator.h"
#include "ch_memory.h"
#include "ch_assert.h"

/* TODO *
 * String pool allocator instead of general heap allocator
 * UTF 8 decoder and encoder
 * Wide String
 * u32 String
 * String viewer (its this or remove rule of 5 and make string memory management manual
 */

#if !defined(CH_STRING_AUTO_MEMORY)
#define CH_STRING_AUTO_MEMORY 0
#else
#undef CH_STRING_AUTO_MEMORY
#define CH_STRING_AUTO_MEMORY 1
#endif

namespace ch {
    usize strlen(const tchar* c_str);

    const tchar endl = '\n';
    const tchar eol = '\n';
    const tchar eof = 0;

    inline bool is_eol(u32 c) {
        return c == '\n' || c == '\r' || c == ch::eof;
    }

    inline bool is_whitespace(u32 c) {
        return c >= '\t' && c <= '\r' || c == ' ';
    }

    /**
     * Base String
     *
     * No copy constructor for a reason
     * Manual memory management unless CH_STRING_AUTO_MEMORY defined
     */
	template <typename T>
	struct Base_String {
        T* data;
        usize count;
        usize allocated;
        ch::Allocator allocator;

        Base_String(const ch::Allocator& in_alloc = ch::get_heap_allocator()) : data(nullptr), count(0), allocated(0), allocator(in_alloc) {}

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

    struct String : public ch::Base_String<tchar> {
        String(const ch::Allocator& in_alloc = ch::get_heap_allocator()) : Base_String<tchar>(in_alloc) { }

        String(const tchar* c_str, const ch::Allocator& in_alloc = ch::get_heap_allocator()) {
            count = ch::strlen(c_str);
            allocated = count + 1;
            allocator = in_alloc;
            data = ch_new(allocator) tchar[allocated];
            ch::memcpy(data, c_str, count);
            data[count] = 0;
        }

        String& operator=(const tchar* c_str) {
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
            ch::memcpy(data, c_str, count);
            return *this;
        }

        operator const tchar*() const {
            return (tchar*)data;
        }

        operator tchar*() const {
            return (tchar*)data;
        }

        bool operator==(const tchar* c_str) const {
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

        String copy(const ch::Allocator& in_alloc = ch::get_heap_allocator()) const {
            String result;
            result.count = count;
            result.allocator = in_alloc;
            result.allocated = allocated;
            result.data = ch_new(in_alloc) tchar[allocated];
            ch::memcpy(result.data, data, count);
            return result;
        }
    };
}
