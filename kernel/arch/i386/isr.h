#pragma once
#include <stdint.h>
#include "interrupts.h"

struct ISRTable {
  using isr = void (*)(Registers*);
  isr isr_table[256];
  bool valid[256] = {0};
  static ISRTable instance;
  void append(isr isr) {
    isr_table[cnt] = isr;
    valid[cnt++] = true;
  }
  void call(int interrupt, Registers* regs) {
    if (valid[interrupt]) isr_table[interrupt](regs);
  }

 private:
  int cnt = 0;
  ISRTable() {
    isr_table[14] = handle_page_fault;
    valid[14] = true;
  }
  ISRTable(ISRTable&&) = delete;
  ISRTable(const ISRTable&) = delete;
};
