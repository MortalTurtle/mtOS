#include <kernel/kernel.h>
#include <kernel/tty.h>
#include <stdint.h>
#include <stdio.h>

extern "C" void kernel_main(uint32_t multiboot_info_phys) {
  terminal_initialize();
  printf("Wazzaap!!\n");
  system_setup(multiboot_info_phys);
  for (;;);
}
