#pragma once
#include <stdint.h>

void init_phys_allocator();
void* alloc_physical_page();
void free_physical_page(void* addr);
