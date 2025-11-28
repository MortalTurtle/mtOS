#pragma once
#include <kernel/bitmap.h>
#include <stdint.h>

template <uint64_t MEM_BEGIN, uint64_t MEM_END, uint32_t PAGE_SIZE>
class page_allocator {
 private:
  static constexpr uint64_t TOTAL_PAGES = (MEM_END - MEM_BEGIN) / PAGE_SIZE;
  static constexpr uint64_t BITMAP_SIZE = (TOTAL_PAGES + 7) / 8;

  bitmap<BITMAP_SIZE> mem_bitmap_;
  uint64_t last_alloc_ = 0;

  static_assert(MEM_END > MEM_BEGIN, "MEM_END must be greater than MEM_BEGIN");
  static_assert(PAGE_SIZE >= 4096, "PAGE_SIZE must be at least 4KB");
  static_assert((MEM_END - MEM_BEGIN) % PAGE_SIZE == 0,
                "Memory range must be page-aligned");

 public:
  void reserve_area(uint64_t start, uint64_t end) {
    if (start < MEM_BEGIN || end > MEM_END) return;

    uint64_t start_page = (start - MEM_BEGIN) / PAGE_SIZE;
    uint64_t end_page = (end - MEM_BEGIN + PAGE_SIZE - 1) / PAGE_SIZE;

    if (end_page > TOTAL_PAGES) end_page = TOTAL_PAGES;

    for (uint64_t i = start_page; i < end_page; i++) {
      mem_bitmap_.bitmap_set(i);
    }
  }

  void* alloc_page() {
    for (uint64_t i = last_alloc_; i < TOTAL_PAGES; i++) {
      if (!mem_bitmap_.bitmap_test(i)) {
        mem_bitmap_.bitmap_set(i);
        last_alloc_ = i + 1;
        return (void*)(i * PAGE_SIZE + MEM_BEGIN);
      }
    }

    for (uint64_t i = 0; i < last_alloc_; i++) {
      if (!mem_bitmap_.bitmap_test(i)) {
        mem_bitmap_.bitmap_set(i);
        last_alloc_ = i + 1;
        return (void*)(i * PAGE_SIZE + MEM_BEGIN);
      }
    }

    return nullptr;  // Out of memory
  }

  void* alloc_pages(uint64_t pages) {
    if (pages == 0) return nullptr;

    for (uint64_t i = last_alloc_; i <= TOTAL_PAGES - pages; i++) {
      bool found = true;
      for (uint64_t j = 0; j < pages; j++) {
        if (mem_bitmap_.bitmap_test(i + j)) {
          found = false;
          break;
        }
      }
      if (found) {
        for (uint64_t j = 0; j < pages; j++) {
          mem_bitmap_.bitmap_set(i + j);
        }
        last_alloc_ = i + pages;
        return (void*)(i * PAGE_SIZE + MEM_BEGIN);
      }
    }
    return nullptr;
  }

  void free_page(void* addr) {
    uint64_t addr_val = (uint64_t)addr;
    if (addr_val < MEM_BEGIN || addr_val >= MEM_END) return;

    uint64_t index = (addr_val - MEM_BEGIN) / PAGE_SIZE;
    if (index < TOTAL_PAGES) {
      mem_bitmap_.bitmap_clear(index);
      if (index < last_alloc_) last_alloc_ = index;
    }
  }

  void free_pages(void* addr, uint64_t pages) {
    if (!addr || pages == 0) return;

    uint64_t addr_val = (uint64_t)addr;
    if (addr_val < MEM_BEGIN || addr_val >= MEM_END) return;

    uint64_t start_index = (addr_val - MEM_BEGIN) / PAGE_SIZE;

    for (uint64_t i = 0; i < pages; i++)
      if (start_index + i < TOTAL_PAGES)
        mem_bitmap_.bitmap_clear(start_index + i);

    if (start_index < last_alloc_) last_alloc_ = start_index;
  }

  uint64_t get_free_pages() const {
    uint64_t count = 0;
    for (uint64_t i = 0; i < TOTAL_PAGES; i++) {
      if (!mem_bitmap_.bitmap_test(i)) count++;
    }
    return count;
  }

  uint64_t get_total_pages() const { return TOTAL_PAGES; }
  uint64_t get_used_pages() const { return TOTAL_PAGES - get_free_pages(); }

  static constexpr uint64_t memory_begin = MEM_BEGIN;
  static constexpr uint64_t memory_end = MEM_END;
  static constexpr uint64_t page_size = PAGE_SIZE;
};
