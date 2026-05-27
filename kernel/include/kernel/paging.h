#pragma once
#include <stdint.h>

#ifdef i386
#define PAGE_SIZE_BYTES 4096
#endif

struct paging_info {
  uint32_t page_size_bytes;
  static paging_info info();
};

extern "C" void init_paging();
extern "C" void* get_physaddr(void* virtualaddr);
extern "C" void map_page(void* physaddr, void* virtualaddr, unsigned int flags);
extern "C" void unmap_page(void* virtualaddr);
extern "C" void paging_load_directory(uint32_t* pgdir);
extern "C" void flush_tlb(void);
