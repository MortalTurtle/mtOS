#include "interrupts.h"
#include <kernel/kernel.h>
#include "paging.h"
#include "physical_alloc.h"

void handle_page_fault(Registers* regs) {
  uint32_t fault_addr;
  asm volatile("mov %%cr2, %0" : "=r"(fault_addr));

  uint32_t error_code = regs->error;
  int present = error_code & 0x1;
  int write = error_code & 0x2;
  int user = error_code & 0x4;
  if (!present) {
    void* page = alloc_physical_page();
    map_page(page, (void*)fault_addr, PAGE_PRESENT | PAGE_RW);
  } else if (user) {
    panic("Segmentation fault");
  }
}
