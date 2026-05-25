#include <kernel/kernellib.h>
#include <kernel/memory.h>
#include <kernel/tty.h>
#include <kernel/virtual_alloc.h>
#include <stdint.h>
#include <stdio.h>

extern "C" void kernel_main(uint32_t multiboot_info_phys) {
  terminal_initialize();
  system_setup(multiboot_info_phys);
  printf("Wazzaap!!\n");
  for (;;);
}
