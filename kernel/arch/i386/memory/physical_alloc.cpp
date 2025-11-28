#include <kernel/kernel.h>
#include <kernel/multiboot.h>
#include <kernel/page_alloc.h>
#include <kernel/physical_alloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define PAGE_SIZE 4096
#define MAX_MEMORY 0xFFFFF000  // 4GB

page_allocator<0, MAX_MEMORY, PAGE_SIZE> page_alloc;

// defined in linker script
extern "C" {
extern uint8_t kernel_physical_start;
extern uint8_t kernel_physical_end;
}

void init_phys_allocator() {
  printf("Initializing physical allocator...\n");
  printf("Kernel loaded at: 0x%x\n", (uint32_t)&kernel_physical_start);
  page_alloc.reserve_area(0x000000, 0x09FFFF);  // Low Memory (0-640K)
  page_alloc.reserve_area(0x0A0000, 0x0BFFFF);  // VGA Video Memory
  page_alloc.reserve_area(0x0C0000, 0x0FFFFF);  // BIOS Area
  uint32_t kernel_start = (uint32_t)&kernel_physical_start;  // 0x200000
  uint32_t kernel_end = (uint32_t)&kernel_physical_end;
  uint32_t kernel_start_page = kernel_start / PAGE_SIZE;
  uint32_t kernel_end_page = (kernel_end + PAGE_SIZE - 1) / PAGE_SIZE;
  printf("Kernel physical range: 0x%x - 0x%x (pages %u-%u)\n", kernel_start,
         kernel_end, kernel_start_page, kernel_end_page);
  page_alloc.reserve_area(kernel_start,
                          kernel_end - 1);  // Kernel Code/Data/BSS
  const multiboot_info* mbi = multiboot_get_info();
  if (mbi->flags & (1 << 3)) {
    printf("Reserving multiboot modules...\n");
    struct multiboot_module* mods =
        (struct multiboot_module*)(mbi->mods_addr + 0xC0000000);
    for (uint32_t i = 0; i < mbi->mods_count; i++) {
      page_alloc.reserve_area(mods[i].mod_start, mods[i].mod_end - 1);
    }
  }
  page_alloc.reserve_area(0xFEC00000, 0xFEC00FFF);  // IOAPIC
  page_alloc.reserve_area(0xFEE00000, 0xFEE00FFF);  // Local APIC
  page_alloc.reserve_area(0xFFFC0000, 0xFFFFFFFF);  // BIOS ROM
  printf("Physical allocator ready.\n");
  printf("Kernel size: %u KB\n", (kernel_end - kernel_start) / 1024);
  printf("Total memory: %u MB\n", MAX_MEMORY / 1024 / 1024);
}

void* alloc_physical_page() {
  auto page = page_alloc.alloc_page();
  printf("allocated phys mem at 0x%x\n", page);
  return page;
}

void free_physical_page(void* addr) {
  printf("deallocate phys mem at 0x%x\n", addr);
  page_alloc.free_page(addr);
}
