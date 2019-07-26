#pragma once

#include "allocator.h"
#include "memory.h"
#include "templates.h"
#include <initializer_list>

namespace ch {
    template <typename T>
    struct Array {
        T* data;
        usize count;
        usize allocated;
        ch::Allocator allocator;

        Array(const ch::Allocator& in_alloc = ch::context_allocator) : data(nullptr), count(0), allocated(0), allocator(in_alloc) {}

        Array(std::initializer_list<T> init_list, const Allocator& in_alloc = ch::context_allocator)
            : data(nullptr), count(0), allocated(0), allocator(in_alloc) {
            reserve(init_list.size());
            for (const T& item : init_list) {
                data[count] = ch::move(item);
                count += 1;
            }
        }

        Array(usize amount, const ch::Allocator& in_alloc = ch::context_allocator)
            : data(nullptr), count(0), allocated(0), allocator(in_alloc) {
            reserve(amount);
        }

        Array<T> copy(const ch::Allocator& in_alloc = ch::context_allocator) const {
            Array<T> result;
            result.count = count;
            result.allocator = in_alloc;
            result.allocated = allocated; 
            result.data = ch_new(result.allocated) T[allocated];
            ch::mem_copy(result.data, data, count * sizeof(T));
            return result;
        }

        void destroy() {
            if (data) {
                assert(allocator && allocated);
                operator ch_delete[](data, allocator);
                data = nullptr;
            }
            count = 0;
            allocated = 0;
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

        explicit operator bool() const { return data && allocated; }
        T& operator[](usize index) {
            assert(index < count);
            return data[index];
        }

        const T& operator[](usize index) const {
            assert(index < count);
            return data[index];
        }

        bool operator==(const Array<T>& right) const {
            if (count != right.count) return false;

            for (usize i = 0; i < count; i++) {
                if (data[i] != right[i]) return false;
            }

            return true;
        }

        bool operator!=(const Array<T>& right) const {
            return !(*this == right);
        }

        void reserve(usize size) {
            // @TODO(CHall): find the best way to preallocate
            allocated += size;

            if (data) {
                data = (T*)allocator.realloc(data, allocated * sizeof(T));
            } else {
                data = ch_new(allocator) T[allocated];
            }
        }

        T& front() {
            return data[0];
        }

        const T& front() const {
            return data[0];
        }

        T& back() {
            return data[count - 1];
        }

        const T& back() const {
            return data[count - 1];
        }

        usize push(const T& t) {
            const usize old_count = count;
            insert(t, count);
            return old_count;
        }

        void pop() {
            remove(count - 1);
        }

        void remove(usize index) {
            assert(index < count);
            ch::mem_move(data + index, data + index + 1, (count - index) * sizeof(T));
            count -= 1;
        }

        void insert(const T& t, usize index) {
            if (count == allocated) {
                reserve(1);
            }

            if (index != count) {
                ch::mem_move(data + index + 1, data + index, (count - index) * sizeof(T));
            }

            data[index] = t;
            count += 1;
        }

        ssize find(const T& t) const {
            for(usize i = 0; i < count; i++) {
                if (data[i] == t) {
                    return i;
                }
            }

            return -1;
        }

        bool contains(const T& t) const {
            return find(t) != -1;
        }

    };
}