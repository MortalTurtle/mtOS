#pragma once

#define PAGE_PRESENT (1 << 0)
#define PAGE_RW (1 << 1)
#define PAGE_SIZE_4MB (1 << 7)

extern "C" void* get_physaddr(void* virtualaddr);
extern "C" void map_page(void* physaddr, void* virtualaddr, unsigned int flags);
