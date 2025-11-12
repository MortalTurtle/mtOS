#include <kernel/multiboot.h>
#include <stdio.h>

const multiboot_info* arch_multiboot_parse(uint32_t multiboot_info_phys) {
  const multiboot_info* mb_info =
      (struct multiboot_info*)(multiboot_info_phys + 0xC0000000);
  printf("Multiboot info at: 0x%p\n", mb_info);
  printf("Memory: lower=%uKB, upper=%uKB\n", mb_info->mem_lower,
         mb_info->mem_upper);
  if (mb_info->flags & MULTIBOOT_INFO_MODS) {
    printf("Modules: %u\n", mb_info->mods_count);
  }
  return mb_info;
}
