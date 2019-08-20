#include "pool_allocator.h"

static void* pool_alloc(const ch::Allocator& allocator, usize size) {
	ch::Pool_Allocator_Header* header = allocator.get_header<ch::Pool_Allocator_Header>();

	ssize start_index = -1;
	usize space_found = 0;
	for (usize i = 0; i < header->buckets.count; i++) {
		ch::Pool_Bucket& it = header->buckets[i];

		if (!it.allocation && start_index == -1) {
			start_index = i;
		}
		else if (it.allocation) {
			start_index = -1;
			space_found = 0;
		}

		if (start_index != -1) {
			space_found += 1;
		}

		if ((space_found * header->bucket_size) >= size) {
			break;
		}
	}

	if (start_index == -1 || (space_found * header->bucket_size) < size) return nullptr;

	void* allocation = (u8*)allocator.data + sizeof(*header) + (start_index * header->bucket_size);

	for (usize i = 0; i < space_found; i++) {
		ch::Pool_Bucket& it = header->buckets[i + start_index];

		it.allocation = allocation;
		it.used = size > header->bucket_size ? header->bucket_size : size;
		size -= header->bucket_size;
	}

	return allocation;
}

static void pool_free(const ch::Allocator& allocator, void* ptr) {
	ch::Pool_Allocator_Header* header = allocator.get_header<ch::Pool_Allocator_Header>();

	// @NOTE(CHall): run through our buckets and reset them if they match out allocation passed in by ptr
	bool found_allocation = false;
	for (ch::Pool_Bucket& it : header->buckets) {
		if (it.allocation == ptr) {
			found_allocation = true;
		}

		if (found_allocation && it.allocation != ptr) return;

		if (found_allocation) {
			it.allocation = nullptr;
			it.used = 0;
		}
	}
}

static void* pool_allocator_func(const ch::Allocator& allocator, void* ptr, usize size) {
	ch::Pool_Allocator_Header* header = allocator.get_header<ch::Pool_Allocator_Header>();

	if (size) {
		if (!ptr) {
			return pool_alloc(allocator, size);
		} else {
			usize old_size = 0;
			bool found_allocation = false;
			for (usize i = 0; i < header->buckets.count; i++) {
				ch::Pool_Bucket& it = header->buckets[i];

				if (it.allocation == ptr) {
					found_allocation = false;
				}

				if (found_allocation && it.allocation != ptr) {
					break;
				}

				// if (old_size + it.used > )
				old_size += it.used;
			}

			// result = ch::realloc(ptr, size);
		}
	} else if (ptr) {
		pool_free(allocator, ptr);
	}

	return nullptr;
}

ch::Allocator ch::make_pool_allocator(usize bucket_size, usize num_buckets) {
	assert(bucket_size);
	assert(num_buckets);

	using Header = ch::Pool_Allocator_Header;
	u8* data = (u8*)ch::malloc(sizeof(Header) + (bucket_size * num_buckets));

	Header* header = (Header*)data;
	*header = {};
	header->buckets.allocator = ch::get_heap_allocator();
	header->buckets.reserve(num_buckets);
	header->buckets.count = num_buckets;
	header->bucket_size = bucket_size;

	for (ch::Pool_Bucket& it : header->buckets) {
		it = {};
	}

	ch::Allocator result;
	result.data = data;
	result.func = pool_allocator_func;
	return result;
}

void ch::free_pool_allocator(Allocator* allocator) {
	ch::Pool_Allocator_Header* header = allocator->get_header<ch::Pool_Allocator_Header>();
	header->buckets.free();
	ch::free(allocator->data);
	allocator->data = nullptr;
}

