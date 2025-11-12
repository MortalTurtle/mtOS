#pragma once
#include <stdint.h>

namespace page_info {
uint32_t page_size_bits();
}

void init_phys_allocator();
void* alloc_physical_page();
void free_physical_page(void* addr);
