#include <kernel/kernel.h>
#include <kernel/tty.h>
#include <stdio.h>

extern "C" void kernel_main(void) {
  system_setup();
  terminal_initialize();
  printf("Wazzaap!!\n");
  for (;;);
}
