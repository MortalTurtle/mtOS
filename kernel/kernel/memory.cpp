#include <kernel/kernellib.h>
#include <kernel/memory.h>
#include <kernel/paging.h>
#include <kernel/virtual_alloc.h>
#include <stdint.h>

enum {
  INIT_PAGES_N = 8,
};

class memory_allocator {
  uint32_t page_size_bytes;
  struct block {
    uint16_t size;
    bool free;
    block* next;
  };
  block* free_list;

 public:
  memory_allocator() {}

  void init() {
    auto info = paging_info::info();
    page_size_bytes = info.page_size_bytes;
    free_list = nullptr;

    void* heap = kvalloc_immediate(INIT_PAGES_N);
    if (heap) {
      free_list = (block*)heap;
      free_list->size = INIT_PAGES_N * page_size_bytes - sizeof(block);
      free_list->free = true;
      free_list->next = nullptr;
    } else
      panic("Failed to allocate memory for allocation");
  }

  void* kmalloc(uint64_t size) {
    if (size == 0) return nullptr;
    size = (size + 7) & ~7;  // 8 bytes aligned

    block* prev = nullptr;
    block* curr = free_list;

    while (curr) {
      if (curr->free && curr->size >= size) {
        if (curr->size >= size + sizeof(block) + 16) {
          block* new_block = (block*)((char*)curr + sizeof(block) + size);
          new_block->size = curr->size - size - sizeof(block);
          new_block->free = true;
          new_block->next = curr->next;

          curr->size = size;
          curr->next = new_block;
        }

        curr->free = false;
        return (char*)curr + sizeof(block);
      }

      prev = curr;
      curr = curr->next;
    }

    uint64_t pages_needed =
        (size + sizeof(block) + page_size_bytes - 1) / page_size_bytes;
    void* new_heap = kvalloc(pages_needed);

    if (!new_heap) return nullptr;

    block* new_block = (block*)new_heap;
    new_block->size = pages_needed * page_size_bytes - sizeof(block);
    new_block->free = true;
    new_block->next = nullptr;

    if (prev)
      prev->next = new_block;
    else
      free_list = new_block;

    return kmalloc(size);
  }

  void kfree(void* ptr) {
    if (!ptr) return;

    block* block_ptr = (block*)((char*)ptr - sizeof(block));
    block_ptr->free = true;

    // TODO: MERGE FREE BLOCKS
  }
};
memory_allocator alloc;

void init_kernel_memory_allocator() { alloc.init(); }

void* kmalloc(uint64_t size) { return alloc.kmalloc(size); }

void kfree(void* addr) { alloc.kfree(addr); }
