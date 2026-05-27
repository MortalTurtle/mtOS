#pragma once
#include <stdint.h>
#include <stdio.h>

#include "interrupts.h"
#include "kernel/kernellib.h"
#include "kernel/proc.h"

struct ISRTable {
  using isr = void (*)(trapframe*);
  isr isr_table[256];
  bool valid[256] = {0};
  static ISRTable instance;
  void append(isr isr) {
    isr_table[cnt] = isr;
    valid[cnt++] = true;
  }
  void call(int interrupt, trapframe* regs) {
    if (valid[interrupt])
      isr_table[interrupt](regs);
    else {
      printf("Unhandled exception: %d\n", interrupt);
      if (regs->cs & 0x3) {
        printf("Killing user process %d\n",
               current_process ? current_process->pid : -1);
        if (current_process) current_process->state = proc_state::Zombie;
        yield();
      } else
        panic("Unhandled kernel exception");
    }
  }

 private:
  int cnt = 0;
  ISRTable() {
    // register interrupt hanlders
    isr_table[14] = handle_page_fault;
    valid[14] = true;
    isr_table[0x80] = syscall_handler;  // 0x80 is syscall interrupt
    valid[0x80] = true;
  }
  ISRTable(ISRTable&&) = delete;
  ISRTable(const ISRTable&) = delete;
};
