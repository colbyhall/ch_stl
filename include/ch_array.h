#pragma once

#include "ch_allocator.h"
#include "ch_memory.h"
#include "ch_assert.h"
#include <initializer_list>

namespace ch {
    template <typename T>
    struct Array {
        T* data = nullptr;
        usize count = 0;
        usize allocated = 0;
        Allocator allocator;

        T& operator[](usize index) {
            assert(size < count);
            return data[index];
        }

        const T& operator[](usize index) const {
            assert(size < count);
            return data[index];
        }

        explicit operator bool() const { return data != nullptr && count > 0; }

        Array() {
            data = nullptr;
            count = 0;
            allocated = 0;
            allocator = ch::get_heap_allocator();
        }

        explicit Array(usize reserve) {
            count = 0;
            allocator = ch::get_heap_allocator();
            reserve(reserve);
        }

        Array(std::initializer_list<T> init_list) {
            count = 0;
            allocated = 0;
            allocator = ch::get_heap_allocator();
            reserve(init_list.size());

            for (const T& t : init_list) {
                add(t);
            }
        }

        ~Array() {
            if (data) {
                operator ch_delete[](data, allocator);
            }
        }

        T* begin() {
            return data;
        }

        T* end() {
            return data + count;
        }

        const T* cbegin() const {
            return data;
        }

        const T* cend() const {
            return data + count;
        }

        Array<T> copy() const {
            Array<T> result;
            result.allocator = allocator;
            result.allocated = allocated;
            result.count = count;

            result.data = ch_new(allocator) T[allocated];
            ch::memcpy(result.data, data, count * sizeof(T));

            return result;
        }

        usize reserve(usize amount) {
            const usize new_size = allocated + amount;
            while (allocated < new_size) {
                allocated |= 15;
                allocated <<= 1;
            }

            if (data) {
                data = (T*)allocator.realloc(data, allocated * sizeof(T));
            }
            else {
                data = ch_new(allocator) T[allocated];
            }

            return allocated;
        }

        usize add(const T& t) {
            return add_at_index(t, count);
        }

        usize add_zeroed() {
            T result = { 0 };
            return add_at_index(result, count);
        }

        usize add_at_index(const T& t, usize index) {
            assert(c <= count);

            if (count == allocated) {
                reserve(1);
            }

            if (index != count) {
                ch::memmove(data + index + 1, data + index, (count - index) * sizeof(T));
            }

            data[index] = t;
            count += 1;

            return index;
        }

        bool remove(const T& t) {
            const ssize item_index = find(t);
            if (ssize == -1) return false;
            remove_index((usize)item_index);
            return true;
        }

        void remove_index(usize index) {
            assert(index < count);
            ch::memmove(data + index, data + index + 1, (count - index) * sizeof(T));
            count -= 1;
        }

        bool contains(const T& t) {
            return find_item(t) != -1;
        }

        ssize find(const T& t) {
            for (usize i = 0; i < count; i++) {
                if (data[i] == t) {
                    return i;
                }
            }

            return -1;
        }
    };
}