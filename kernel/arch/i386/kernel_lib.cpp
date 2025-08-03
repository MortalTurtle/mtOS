#include <kernel/kernel.h>
#include <stdio.h>

extern "C"
__attribute__((noreturn))
void panic(char* msg) {
  printf("Kernel panic: %s", msg);
  __asm__ volatile (
    "cli\n"
    "hlt"
  );
}