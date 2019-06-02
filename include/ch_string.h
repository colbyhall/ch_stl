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

#define CH_EOF 0

namespace ch {
    usize strlen(const char* c_str);

    inline bool is_eol(u32 c) {
        return c == '\n' || c == '\r' || c == CH_EOF;
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

#if CH_STRING_AUTO_MEMORY
        Base_String(const Base_String<T>& copy) 
            : count(copy.count), allocated(copy.allocated), allocator(copy.allocator) {
            if (!allocated) {
                data = nullptr;
                return;
            }

            data = ch_new(allocator) T[allocated];
            ch::memcpy(data, copy.data, count * sizeof(T));
        }

        Base_String(Base_String<T>&& move)
            : data(move.data), count(move.count), allocated(move.allocated), allocator(move.allocator) {
            move.data = nullptr;
            move.count = 0;
            move.allocated = 0;
        }

        ~Base_String() {
            if (data) {
                assert(allocated && allocator);
                operator ch_delete[](data, allocator);
            }
        }

        Base_String<T>& operator=(const Base_String<T>& copy) {
            if (data) {
                assert(allocated && allocator);
                operator ch_delete[](data, allocator);
            }
            count = copy.count;
            allocated = copy.allocated;
            allocator = copy.allocator;
            data = ch_new(allocator) T[allocated];
            ch::memcpy(data, copy.data, count * sizeof(T));
            return *this;
        }
        Base_String<T>& operator=(Base_String<T>&& move) {
            data = move.data;
            count = move.count;
            allocated = move.allocated;
            allocator = move.allocator;

            move.data = nullptr;
            move.count = 0;
            move.allocated = 0;
            return *this;
        }
#endif

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
        
        bool destroy() {
            if (data) {
                assert(allocated && allocator);
                operator ch_delete[](data, allocator);
                data = nullptr;
            }
            count = 0;
            allocated = 0;
        }

#if !CH_STRING_AUTO_MEMORY
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
#endif
	};

    struct String : public ch::Base_String<u8> {
        String(const char* c_str, const ch::Allocator& in_alloc = ch::get_heap_allocator()) {
            count = ch::strlen(c_str);
            allocated = count + 1;
            allocator = in_alloc;
            data = ch_new(allocator) u8[allocated];
            ch::memcpy(data, c_str, count);
            data[count] = 0;
        }

        String& operator=(const char* c_str) {
            const usize c_str_count = ch::strlen(c_str);
            if (c_str_count + 1 > allocated) {
                if (data) {
                    assert(allocator);
                    operator ch_delete[](data, allocator);
                }
                allocated = c_str_count + 1;
                data = ch_new(allocator) u8[allocated];
            }

            count = c_str_count;
            ch::memcpy(data, c_str, count);
            return *this;
        }

        operator const char*() const {
            return (char*)data;
        }

        operator char*() const {
            return (char*)data;
        }

        bool operator==(const char* c_str) const {
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
    };
}