#pragma once
#include <stdint.h>

struct Registers {
  uint32_t ds;
  uint32_t edi, esi, ebp, kern_esp, ebx, edx, ecx, eax;
  uint32_t interrupt, error;
  uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed));

struct ISRTable {
  using isr = void(*)(Registers*);
  isr isr_table[256];
  bool valid[256] = {0};
  static ISRTable instance;
  void append(isr isr) {
    isr_table[cnt] = isr;
    valid[cnt++] = true;
  }
  void call(int interrupt, Registers* regs) {
    if (valid[interrupt])
      isr_table[interrupt](regs);
  }
  private:
    int cnt = 0;
    ISRTable(){}
    ISRTable(ISRTable&&) = delete;
    ISRTable(const ISRTable&) = delete;
};