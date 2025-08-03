#include "paging.h"
#include <stdint.h>

#define PAGE_PRESENT (1 << 0)
#define PAGE_RW (1 << 1)
#define PAGE_SIZE_4MB (1 << 7)

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

extern "C" void init_paging() {
  page_directory[0] = (uint32_t)first_page_table | PAGE_PRESENT | PAGE_RW;
  for (int i = 1; i < 1024; ++i) page_directory[i] = 0;
  for (int i = 0; i < 1024; ++i)
    first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_RW;
}

extern "C" void* get_physaddr(void* virtualaddr) {
  uint32_t pdindex = (uint32_t)virtualaddr >> 22;
  uint32_t ptindex = (uint32_t)virtualaddr >> 12 & 0x03FF;

  uint32_t* pd = (uint32_t*)0xFFFFF000;
  // Here you need to check whether the PD entry is present.

  uint32_t* pt = ((uint32_t*)0xFFC00000) + (0x400 * pdindex);
  // Here you need to check whether the PT entry is present.

  return (void*)((pt[ptindex] & ~0xFFF) + ((uint32_t)virtualaddr & 0xFFF));
}

extern "C" void map_page(void* physaddr, void* virtualaddr,
                         unsigned int flags) {
  // Make sure that both addresses are page-aligned.

  unsigned long pdindex = (unsigned long)virtualaddr >> 22;
  unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

  unsigned long* pd = (unsigned long*)0xFFFFF000;
  // Here you need to check whether the PD entry is present.
  // When it is not present, you need to create a new empty PT and
  // adjust the PDE accordingly.

  unsigned long* pt = ((unsigned long*)0xFFC00000) + (0x400 * pdindex);
  // Here you need to check whether the PT entry is present.
  // When it is, then there is already a mapping present. What do you do now?

  pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01;  // Present

  // Now you need to flush the entry in the TLB
  // or you might not notice the change.
}
