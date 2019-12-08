#pragma once

#include "allocator.h"

namespace ch {
	const usize default_gap_size = 256;

	template <typename T>
	struct Gap_Buffer {
		T* data = nullptr;
		usize allocated = 0;
		T* gap = nullptr;
		usize gap_size = 0;
		ch::Allocator allocator;

		CH_FORCEINLINE usize count() const { return allocated - gap_size; }

		CH_FORCEINLINE T& operator[](usize index) {
			assert(index < count());

			if (data + index < gap) {
				return data[index];
			}
			else {
				return data[index + gap_size];
			}
		}

		CH_FORCEINLINE T operator[](usize index) const {
			assert(index < count());

			if (data + index < gap) {
				return data[index];
			}
			else {
				return data[index + gap_size];
			}
		}

		CH_FORCEINLINE operator bool() const {
			return data && count();
		}

		Gap_Buffer(const ch::Allocator& in_alloc = ch::context_allocator) : allocator(in_alloc) {}
		Gap_Buffer(usize size, const ch::Allocator& in_alloc = ch::context_allocator) : allocator(in_alloc) {
			if (size < default_gap_size) size = default_gap_size;
			allocated = size;
			data = (T*)allocator.alloc(allocated * sizeof(T));
			assert(data);

			gap = data;
			gap_size = size;
		}

		void free() {
			if (data) allocator.free(data);
		}

		void resize(usize new_gap_size) {
			if (!data) {
				*this = Gap_Buffer(new_gap_size, allocator);
				return;
			}
			assert(data);
			assert(gap_size == 0);

			const usize old_size = allocated;
			const usize new_size = old_size + new_gap_size;

			T* old_data = data;
			T* new_data = (T*)allocator.realloc(old_data, new_size * sizeof(T));
			assert(new_data);

			data = new_data;
			allocated = new_size;
			gap = (data + old_size);
			gap_size = new_gap_size;
		}

		void move_gap_to_index(usize index) {
			assert(index <= allocated);

			T* index_ptr = get_index_as_cursor(index);
			if (index_ptr == gap) return;

			if (index_ptr < gap) {
				const usize amount_to_move = gap - index_ptr;
				ch::mem_move(gap + gap_size - amount_to_move, index_ptr, amount_to_move * sizeof(T));
				gap = index_ptr;
			} else {
				const usize amount_to_move = index_ptr - (gap + gap_size);
				ch::mem_move(gap, gap + gap_size, amount_to_move * sizeof(T));
				gap += amount_to_move;
			}
		}

		T* get_index_as_cursor(usize index) {
			if (data + index <= gap) {
				return data + index;
			}

			return data + gap_size + index;
		}

		void insert(T c, usize index) {
			if (gap_size <= 0) {
				resize(default_gap_size);
			}

			move_gap_to_index(index);

			T* cursor = get_index_as_cursor(index);

			*cursor = c;
			gap += 1;
			gap_size -= 1;
		}

		void push(T c) {
			if (!data || !gap) {
				insert(c, 0);
			} else {
				insert(c, allocated - gap_size);
			}
		}

		void remove_at_index(usize index) {
			const usize buffer_count = count();
			assert(index < buffer_count);

			move_gap_to_index(index + 1);

			gap -= 1;
			gap_size += 1;
		}

		void remove_between(usize index, usize count) {
			// @SPEED(CHall): this is slow
			for (usize i = index; i < index + count; i++) {
				remove_at_index(i);
			}
		}
	};
}