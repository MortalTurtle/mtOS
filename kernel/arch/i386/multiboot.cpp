#include <kernel/multiboot.h>
#include <stdio.h>

static uint32_t saved_phys_addr = 0;

const multiboot_info* arch_multiboot_parse(uint32_t multiboot_info_phys) {
  saved_phys_addr = multiboot_info_phys;

  uint32_t cr0;
  asm volatile("mov %%cr0, %0" : "=r"(cr0));
  bool paging_enabled = (cr0 & 0x80000000) != 0;

  const multiboot_info* mb_info;

  if (paging_enabled)
    mb_info = (multiboot_info*)(multiboot_info_phys + 0xC0000000);
  else
    mb_info = (multiboot_info*)multiboot_info_phys;

  if (mb_info->flags == 0 || mb_info->flags == 0xFFFFFFFF) {
    printf("WARNING: Invalid multiboot info at 0x%p\n", mb_info);
    return nullptr;
  }

  return mb_info;
}
