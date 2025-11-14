#include <kernel/paging.h>
#include <kernel/physical_alloc.h>
#include <stdint.h>
#include "paging_defs.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

extern "C" void paging_load_directory(uint32_t*);
extern "C" void enable_paging();

extern "C" void init_paging() {
  page_directory[0] = (uint32_t)first_page_table | PAGE_PRESENT | PAGE_RW;
  for (int i = 0; i < 1024; ++i)
    first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_RW;
  for (int i = 1; i < 1024; ++i) page_directory[i] = 0;
  page_directory[1023] = (uint32_t)page_directory | PAGE_PRESENT | PAGE_RW;
  page_directory[768] = (uint32_t)first_page_table | PAGE_PRESENT | PAGE_RW;

  paging_load_directory(page_directory);
  enable_paging();
}

void flush_tlb_single(uint32_t addr) {
  asm volatile("invlpg (%0)" ::"r"(addr) : "memory");
}

extern "C" void flush_tlb() {
  asm volatile("mov %%cr3, %%eax; mov %%eax, %%cr3" ::: "eax");
}

paging_info paging_info::info() { return {4096}; }

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
    uint32_t* new_pt = (uint32_t*)alloc_physical_page();
    if (!new_pt) return;  // Handle allocation failure
    for (int i = 0; i < 1024; i++) new_pt[i] = 0;
    pd[pdindex] = (uint32_t)new_pt | PAGE_PRESENT | PAGE_RW;
  }

  uint32_t* pt = (uint32_t*)(0xFFC00000 + (pdindex << 12));
  pt[ptindex] = (uint32_t)physaddr | (flags & 0xFFF) | PAGE_PRESENT;  // Present

  flush_tlb_single((uint32_t)virtualaddr);
}

extern "C" void unmap_page(void* virtualaddr) {
  if ((uint32_t)virtualaddr & 0xFFF) return;
  unsigned long pdindex = (unsigned long)virtualaddr >> 22;
  unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
  uint32_t* pd = (uint32_t*)0xFFFFF000;

  if (!(pd[pdindex] & PAGE_PRESENT)) return;
  uint32_t* pt = (uint32_t*)(0xFFC00000 + (pdindex << 12));

  if (!(pt[ptindex] & PAGE_PRESENT)) return;
  pt[ptindex] = 0;

  flush_tlb_single((uint32_t)virtualaddr);
}
