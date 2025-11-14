#include <kernel/kernel.h>
#include <kernel/multiboot.h>
#include <kernel/paging.h>
#include <kernel/physical_alloc.h>
#include <kernel/virtual_alloc.h>

void system_setup(uint32_t multiboot_info_phys) {
  init_paging();
  multiboot_parse(multiboot_info_phys);
  init_phys_allocator();
  init_virtual_allocator();
}
