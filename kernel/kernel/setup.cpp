#include <kernel/kernel.h>
#include <kernel/multiboot.h>
#include <kernel/physical_alloc.h>

void system_setup(uint32_t multiboot_info_phys) {
  multiboot_parse(multiboot_info_phys);
  init_phys_allocator();
}
