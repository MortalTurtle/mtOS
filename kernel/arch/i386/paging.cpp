#include "paging.h"
#include <kernel/memory_manager.h>
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
  page_directory[1023] = (uint32_t)page_directory | PAGE_PRESENT | PAGE_RW;

  for (int i = 1; i < 1022; i++) {
    page_directory[i] = 0;
  }
}

void flush_tlb_single(uint32_t addr) {
  asm volatile("invlpg (%0)" ::"r"(addr) : "memory");
}

extern "C" void* get_physaddr(void* virtualaddr) {
  uint32_t pdindex = (uint32_t)virtualaddr >> 22;
  uint32_t ptindex = (uint32_t)virtualaddr >> 12 & 0x03FF;

  uint32_t* pd = (uint32_t*)0xFFFFF000;
  if (!(pd[pdindex] & PAGE_PRESENT)) return nullptr;

  // Access page table via recursive mapping
  uint32_t* pt = (uint32_t*)(0xFFC00000 + (pdindex << 12));
  if (!(pt[ptindex] & PAGE_PRESENT)) return nullptr;

  return (void*)((pt[ptindex] & ~0xFFF) + ((uint32_t)virtualaddr & 0xFFF));
}

extern "C" void map_page(void* physaddr, void* virtualaddr,
                         unsigned int flags) {
  // Make sure that both addresses are page-aligned.
  if ((uint32_t)physaddr & 0xFFF || (uint32_t)virtualaddr & 0xFFF) return;
  unsigned long pdindex = (unsigned long)virtualaddr >> 22;
  unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

  uint32_t* pd = (uint32_t*)0xFFFFF000;
  if (!(pd[pdindex] & PAGE_PRESENT)) {
    // Create new page table (must be allocated and initialized to zero)
    uint32_t* new_pt = (uint32_t*)kmalloc(4096);
    if (!new_pt) return;  // Handle allocation failure
    for (int i = 0; i < 1024; i++) new_pt[i] = 0;
    pd[pdindex] = (uint32_t)new_pt | PAGE_PRESENT | PAGE_RW;
  }

  uint32_t* pt = (uint32_t*)(0xFFC00000 + (pdindex << 12));
  pt[ptindex] = (uint32_t)physaddr | (flags & 0xFFF) | PAGE_PRESENT;  // Present

  flush_tlb_single((uint32_t)virtualaddr);
}
