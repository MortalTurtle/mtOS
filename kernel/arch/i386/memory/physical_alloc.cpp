#include <kernel/kernel.h>
#include <kernel/multiboot.h>
#include <kernel/physical_alloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PAGE_SIZE 4096
#define MAX_MEMORY 0xFFFFFFFF  // 4GB
#define BITMAP_SIZE (MAX_MEMORY / PAGE_SIZE / 8)

uint8_t phys_bitmap[BITMAP_SIZE];
uint32_t total_pages;
uint32_t last_alloc = 0;

void bitmap_clear(uint32_t index) {
  if (index >= total_pages) return;
  uint32_t byte = index / 8;
  uint32_t bit = index % 8;
  phys_bitmap[byte] &= ~(1 << bit);
}

void bitmap_set(uint32_t index) {
  if (index >= total_pages) return;
  uint32_t byte = index / 8;
  uint32_t bit = index % 8;
  phys_bitmap[byte] |= (1 << bit);
}

int bitmap_test(uint32_t index) {
  if (index >= total_pages) return 1;
  uint32_t byte = index / 8;
  uint32_t bit = index % 8;
  return phys_bitmap[byte] & (1 << bit);
}

void reserve_area(uint32_t start, uint32_t end, const char* description) {
  uint32_t start_page = start / PAGE_SIZE;
  uint32_t end_page = (end + PAGE_SIZE - 1) / PAGE_SIZE;

  if (end_page > total_pages) end_page = total_pages;

  printf("  Reserve: 0x%x-0x%x (%s) pages %u-%u\n", start, end, description,
         start_page, end_page);

  for (uint32_t i = start_page; i < end_page; i++) {
    bitmap_set(i);
  }
}

// defined in linker script
extern "C" {
extern uint8_t kernel_physical_start;
extern uint8_t kernel_physical_end;
}

extern "C" void init_phys_allocator() {
  total_pages = MAX_MEMORY / PAGE_SIZE;
  memset(phys_bitmap, 0, BITMAP_SIZE);
  printf("Initializing physical allocator...\n");
  printf("Kernel loaded at: 0x%x\n", (uint32_t)&kernel_physical_start);
  reserve_area(0x000000, 0x09FFFF, "Low Memory (0-640K)");
  reserve_area(0x0A0000, 0x0BFFFF, "VGA Video Memory");
  reserve_area(0x0C0000, 0x0FFFFF, "BIOS Area");
  uint32_t kernel_start = (uint32_t)&kernel_physical_start;  // 0x200000
  uint32_t kernel_end = (uint32_t)&kernel_physical_end;
  uint32_t kernel_start_page = kernel_start / PAGE_SIZE;
  uint32_t kernel_end_page = (kernel_end + PAGE_SIZE - 1) / PAGE_SIZE;
  printf("Kernel physical range: 0x%x - 0x%x (pages %u-%u)\n", kernel_start,
         kernel_end, kernel_start_page, kernel_end_page);
  reserve_area(kernel_start, kernel_end - 1, "Kernel Code/Data/BSS");
  const multiboot_info* mbi = multiboot_get_info();
  if (mbi->flags & (1 << 3)) {
    printf("Reserving multiboot modules...\n");
    struct multiboot_module* mods =
        (struct multiboot_module*)(mbi->mods_addr + 0xC0000000);
    for (uint32_t i = 0; i < mbi->mods_count; i++) {
      reserve_area(mods[i].mod_start, mods[i].mod_end - 1, "Multiboot Module");
    }
  }
  reserve_area(0xFEC00000, 0xFEC00FFF, "IOAPIC");
  reserve_area(0xFEE00000, 0xFEE00FFF, "Local APIC");
  reserve_area(0xFFFC0000, 0xFFFFFFFF, "BIOS ROM");
  last_alloc = kernel_end_page;
  printf("Physical allocator ready.\n");
  printf("First free page: 0x%x\n", last_alloc * PAGE_SIZE);
  printf("Kernel size: %u KB\n", (kernel_end - kernel_start) / 1024);
  printf("Total memory: %u MB\n", MAX_MEMORY / 1024 / 1024);
}

extern "C" void* alloc_physical_page() {
  for (uint32_t i = last_alloc; i < total_pages; i++) {
    if (!bitmap_test(i)) {
      bitmap_set(i);
      last_alloc = i + 1;
      return (void*)(i * PAGE_SIZE);
    }
  }
  for (uint32_t i = 0; i < last_alloc; i++) {
    if (!bitmap_test(i)) {
      bitmap_set(i);
      last_alloc = i + 1;
      return (void*)(i * PAGE_SIZE);
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
