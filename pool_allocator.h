#pragma once

#include "hash_table.h"

namespace ch {
	
	struct Pool_Bucket {
		void* allocation;
		usize used;
	};

	struct Pool_Allocator_Header {
		ch::Array<Pool_Bucket> buckets;
		usize bucket_size;

		CH_FORCEINLINE usize get_num_buckets() const { return buckets.count; }
	};

	ch::Allocator make_pool_allocator(usize bucket_size, usize num_buckets);
	void free_pool_allocator(ch::Allocator* allocator);

}