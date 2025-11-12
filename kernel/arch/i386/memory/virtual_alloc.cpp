#include <kernel/memory.h>
#include <kernel/page_alloc.h>

#define USER_SPACE_START 0x00000000
#define USER_SPACE_END 0xBFFFF000  // 3GB aligned
#define KERNEL_SPACE_START 0xC0000000
#define KERNEL_SPACE_END 0xFFFFF000  // 1GB aligned

#define PAGE_SIZE 4096

#define USER_PAGES_COUNT \
  ((USER_SPACE_END - USER_SPACE_START + 1) / PAGE_SIZE)  // 786432
#define KERNEL_PAGES_COUNT \
  ((KERNEL_SPACE_END - KERNEL_SPACE_START + 1) / PAGE_SIZE)  // 262144

#define USER_BITMAP_SIZE ((USER_PAGES_COUNT + 7) / 8)      // ~96KB
#define KERNEL_BITMAP_SIZE ((KERNEL_PAGES_COUNT + 7) / 8)  // ~32KB

page_allocator<USER_SPACE_START, USER_SPACE_END, PAGE_SIZE> user_v_page_alloc;
page_allocator<KERNEL_SPACE_START, KERNEL_SPACE_END, PAGE_SIZE>
    kernel_v_page_alloc;

void* valloc(uint64_t pages) {}

void vfree(void* addr, uint64_t pages) {}

void* kvalloc(uint64_t pages) {}
void kvfree(void* addr, uint64_t pages) {}
