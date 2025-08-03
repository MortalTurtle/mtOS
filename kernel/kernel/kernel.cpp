#include <kernel/tty.h>
#include <stdio.h>

extern "C" void kernel_main(void) {
  terminal_initialize();
  printf("Wazzaap!!\n");
  for (;;);
}
