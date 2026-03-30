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
  int* ints = (int*)kvalloc(1);
  if (ints == nullptr) {
    panic("NO MEMORY");
  }
  ints[0] = 1;
  ints[1] = 2;
  ints[2] = 3;
  ints[3] = 4;
  for (int i = 0; i < 4; ++i) printf("%d\n", ints[i]);
  for (;;);
}
