#include <kernel/kernellib.h>

extern "C" __attribute__((noreturn)) void panic(char* msg) {
  uint16_t* vga = (uint16_t*)0xB8000;
  const char* prefix = "KERNEL PANIC: ";
  int pos = 0;
  for (int i = 0; i < 80 * 25; i++) {
    vga[i] = ' ' | (0x4F << 8);
  }
  for (int i = 0; prefix[i]; i++) {
    vga[pos++] = prefix[i] | (0x4F << 8);
  }
  for (int i = 0; msg[i]; i++) {
    vga[pos++] = msg[i] | (0x4F << 8);
  }
  __asm__ volatile("cli; hlt");
  __builtin_unreachable();
}
