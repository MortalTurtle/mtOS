#pragma once
#include <stdint.h>

struct paging_info {
  uint32_t page_size_bits;
  static paging_info info();
};

extern "C" void init_paging();
extern "C" void* get_physaddr(void* virtualaddr);
extern "C" void map_page(void* physaddr, void* virtualaddr, unsigned int flags);
extern "C" void unmap_page(void* virtualaddr);
