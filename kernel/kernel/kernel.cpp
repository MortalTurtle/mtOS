#include <kernel/kernellib.h>
#include <kernel/memory.h>
#include <kernel/tty.h>
#include <kernel/virtual_alloc.h>
#include <stdint.h>
#include <stdio.h>

extern "C" void kernel_main(uint32_t multiboot_info_phys) {
  terminal_initialize();
  printf("Wazzaap!!\n");
  system_setup(multiboot_info_phys);
  for (int i = 0; i < 1000; ++i) {
    int* ints = (int*)kmalloc(16);
  }
  // ints[0] = 1;
  // ints[1] = 2;
  // ints[2] = 3;
  // ints[3] = 4;
  // for (int i = 0; i < 4; ++i) printf("%d\n", ints[i]);
  printf("Wazzaap!!\n");
  for (;;);
}
