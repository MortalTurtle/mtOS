#pragma once
#include <stdint.h>

namespace page_info {
uint64_t page_size_bytes();
}

extern "C" void init_phys_allocator();
extern "C" void* alloc_physical_page();
extern "C" void free_physical_page(void* addr);
