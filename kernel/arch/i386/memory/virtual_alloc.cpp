#include <kernel/memory.h>
#include <kernel/page_alloc.h>
#include <kernel/paging.h>
#include <kernel/physical_alloc.h>
#include <stdio.h>

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

void init_virtual_allocator() {
  user_v_page_alloc.reserve_area(0x00000000, 0x003FFFFF);
  kernel_v_page_alloc.reserve_area(0xC0000000,
                                   0xC03FFFFF);  // kerhel higher half begin

  kernel_v_page_alloc.reserve_area(0xFFFFF000,
                                   0xFFFFF000);  // recursive mapping
  uint32_t pd_addr = 0xFFFFF000;
  uint32_t pt_addr = 0xFFC00000;
  kernel_v_page_alloc.reserve_area(pd_addr, pd_addr + PAGE_SIZE - 1);
  kernel_v_page_alloc.reserve_area(pt_addr, pt_addr + PAGE_SIZE - 1);
  user_v_page_alloc.reserve_area(0x00000000,
                                 0x00000FFF);  // NULL pointer protection
  user_v_page_alloc.reserve_area(
      0x000A0000, 0x000FFFFF);  // Video memory and BIOS (0xA0000-0xFFFFF)
  printf("Virtual allocator initialized:\n");
  printf("  User space: %u free / %u total pages\n",
         user_v_page_alloc.get_free_pages(),
         user_v_page_alloc.get_total_pages());
  printf("  Kernel space: %u free / %u total pages\n",
         kernel_v_page_alloc.get_free_pages(),
         kernel_v_page_alloc.get_total_pages());
  printf("  Reserved areas:\n");
  printf("    - Identity mapping: 0x0-0x3FFFFF (user)\n");
  printf("    - Kernel mapping: 0xC0000000-0xC03FFFFF\n");
  printf("    - Page structures: 0xFFFFF000, 0xFFC00000\n");
}
void* valloc(uint64_t pages) {
  auto page = user_v_page_alloc.alloc_pages(pages);
  if (!page) return nullptr;
  printf("allocated virt mem at 0x%x\n", (uint32_t)page);
  return page;
}

void vfree(void* addr, uint64_t pages) {
  for (uint64_t i = 0; i < pages; i++) {
    void* page_virt = (void*)((uint32_t)addr + i * PAGE_SIZE);
    printf("deallocated virt mem at 0x%x\n", (uint32_t)page_virt);
    void* phys_addr = get_physaddr(page_virt);
    unmap_page(page_virt);
    if (phys_addr) free_physical_page(phys_addr);
  }
  user_v_page_alloc.free_pages(addr, pages);
}

void* kvalloc(uint64_t pages) {
  auto page = kernel_v_page_alloc.alloc_pages(pages);
  printf("allocated virt mem at 0x%x\n", (uint32_t)page);
  return page;
}

void kvfree(void* addr, uint64_t pages) {
  for (uint64_t i = 0; i < pages; i++) {
    void* page_virt = (void*)((uint32_t)addr + i * PAGE_SIZE);
    printf("deallocated virt mem at 0x%x\n", page_virt);
    void* phys_addr = get_physaddr(page_virt);
    unmap_page(page_virt);
    if (phys_addr) free_physical_page(phys_addr);
  }
  kernel_v_page_alloc.free_pages(addr, pages);
}
