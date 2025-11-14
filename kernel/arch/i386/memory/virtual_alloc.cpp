#include <kernel/memory.h>
#include <kernel/page_alloc.h>
#include <kernel/paging.h>
#include <kernel/physical_alloc.h>

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

void init_virtual_allocator() {}

void* valloc(uint64_t pages) { return user_v_page_alloc.alloc_pages(pages); }

void vfree(void* addr, uint64_t pages) {
  for (uint64_t i = 0; i < pages; i++) {
    void* page_virt = (void*)((uint32_t)addr + i * PAGE_SIZE);
    void* phys_addr = get_physaddr(page_virt);
    unmap_page(page_virt);
    if (phys_addr) free_physical_page(phys_addr);
  }
  user_v_page_alloc.free_pages(addr, pages);
}

void* kvalloc(uint64_t pages) { return kernel_v_page_alloc.alloc_pages(pages); }

void kvfree(void* addr, uint64_t pages) {
  for (uint64_t i = 0; i < pages; i++) {
    void* page_virt = (void*)((uint32_t)addr + i * PAGE_SIZE);
    void* phys_addr = get_physaddr(page_virt);
    unmap_page(page_virt);
    if (phys_addr) free_physical_page(phys_addr);
  }
  kernel_v_page_alloc.free_pages(addr, pages);
}
