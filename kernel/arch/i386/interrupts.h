#pragma once
#include <stdint.h>

struct Registers {
  uint32_t ds;
  uint32_t edi, esi, ebp, kern_esp, ebx, edx, ecx, eax;
  uint32_t interrupt, error;
  uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed));

void handle_page_fault(Registers* regs);
