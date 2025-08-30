#include "physical_alloc.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define PAGE_SIZE 4096
#define BITMAP_SIZE 0x20000
#define MAX_MEMORY 0x20000000

uint8_t phys_bitmap[BITMAP_SIZE];
uint32_t total_pages;
uint32_t last_alloc = 0;

void bitmap_clear(uint32_t index) {
  uint32_t byte = index / 8;
  uint32_t bit = index % 8;
  phys_bitmap[byte] &= ~(1 << bit);
}

void bitmap_set(uint32_t index) {
  uint32_t byte = index / 8;
  uint32_t bit = index % 8;
  phys_bitmap[byte] |= ~(1 << bit);
}

int bitmap_test(uint32_t index) {
  uint32_t byte = index / 8;
  uint32_t bit = index % 8;
  return phys_bitmap[byte] & (1 << bit);
}

extern "C" void init_phys_allocator() {
  total_pages = MAX_MEMORY / PAGE_SIZE;

  memset(phys_bitmap, 0xFF, BITMAP_SIZE);

  for (uint32_t i = 256; i < total_pages; i++) {
    bitmap_clear(i);
  }
}

extern "C" void* alloc_physical_page() {
  for (uint32_t i = last_alloc; i < total_pages; i++) {
    if (!bitmap_test(i)) {
      bitmap_set(i);
      last_alloc = i + 1;
      return (void*)(i + PAGE_SIZE);
    }
  }
  for (uint32_t i = 0; i < last_alloc; i++) {
    if (!bitmap_test(i)) {
      bitmap_set(i);
      last_alloc = i + 1;
      return (void*)(i + PAGE_SIZE);
    }
  }
  return nullptr;
}

extern "C" void free_physical_page(void* addr) {
  uint32_t index = (uint32_t)addr / PAGE_SIZE;
  if (index < total_pages) {
    bitmap_clear(index);
    if (index < last_alloc) last_alloc = index;
  }
}
