#pragma once

#include "ch_allocator.h"
#include "ch_memory.h"
#include "ch_templates.h"
#include "ch_assert.h"
#include <initializer_list>

namespace ch {
    template <typename T>
    struct Array {
        T* data;
        usize count;
        usize allocated;
        Allocator allocator;

        Array(const Allocator& in_alloc = ch::get_heap_allocator()) : data(nullptr), count(0), allocated(0), allocator(in_alloc) {}

        Array(std::initializer_list<T> init_list, const Allocator& in_alloc = ch::get_heap_allocator()) 
            : data(nullptr), count(0), allocated(0), allocator(in_alloc) {
            reserve(init_list.size());
            for (const T& item : init_list) {
                data[count] = ch::move(item);
                count += 1;
            }
        }

        Array(usize amount, const Allocator& in_alloc = ch::get_heap_allocator())
            : data(nullptr), count(0), allocated(0), allocator(in_alloc) {
            reserve(amount);
        }
        
        Array(const Array<T>& copy) 
            : data(nullptr), count(copy.count), allocated(0), allocator(copy.allocator) {
            reserve(copy.allocated);
            ch::memcpy(data, copy.data, count * sizeof(T));
        }

        Array(Array<T>&& move)
            : data(move.data), count(move.count), allocated(move.allocated), allocator(move.allocator) {
            move.data = nullptr;
            move.count = 0;
            move.allocated = 0;
        }

        Array<T>& operator=(const Array<T>& copy) {
            if (data) {
                assert(allocator && allocated);
                operator ch_delete[](data, allocator);
                data = nullptr;
            }
            allocated = 0;
            count = copy.count;
            reserve(count);
            ch::memcpy(data, copy.data, count * sizeof(T));
            return *this;
        }

        Array<T>& operator=(Array<T>&& move) {
            if (data) {
                assert(allocator && allocated);
                operator ch_delete[](data, allocator);
            }
            allocator = move.allocator;
            allocated = move.allocated;
            count = move.count;
            data = move.data;

            move.data = nullptr;
            move.count = 0;
            move.allocated = 0;
            return *this;
        }

        ~Array() {
            assert(allocator);
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
            ch::memmove(data + index, data + index + 1, (count - index) * sizeof(T));
            count -= 1;
        }

        void insert(const T& t, usize index) {
            if (count == allocated) {
                reserve(1);
            }

            if (index != count) {
                ch::memmove(data + index + 1, data + index, (count - index) * sizeof(T));
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