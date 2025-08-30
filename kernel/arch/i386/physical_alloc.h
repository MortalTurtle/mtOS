#pragma once

extern "C" void init_phys_allocator();
extern "C" void* alloc_physical_page();
extern "C" void free_physical_page(void* addr);
