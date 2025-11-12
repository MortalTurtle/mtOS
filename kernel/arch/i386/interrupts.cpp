#include "interrupts.h"
#include <kernel/kernel.h>
#include <kernel/physical_alloc.h>
#include "memory/paging.h"

void handle_user_oom(uint32_t fault_addr, Registers* regs) {
  void* phys = alloc_physical_page();
  if (phys)
    map_page(phys, (void*)fault_addr, PAGE_PRESENT | PAGE_RW | PAGE_USER);
  else
    panic("User OOM");
}

void handle_kernel_oom(uint32_t fault_addr, Registers* regs) {
  void* phys = alloc_physical_page();
  if (phys)
    map_page(phys, (void*)fault_addr, PAGE_PRESENT | PAGE_RW);
  else
    panic("Kernel OOM");
}

void handle_page_fault(Registers* regs) {
  uint32_t fault_addr;
  asm volatile("mov %%cr2, %0" : "=r"(fault_addr));

  uint32_t error_code = regs->error;
  int present = error_code & 0x1;
  int write = error_code & 0x2;
  int user = error_code & 0x4;
  if (!present) {
    void* page = alloc_physical_page();
    if (page) {
      uint32_t flags = PAGE_PRESENT | PAGE_RW;
      if (user) flags |= PAGE_USER;
      map_page(page, (void*)fault_addr, flags);
      return;
    }
    if (user)
      handle_user_oom(fault_addr, regs);
    else
      handle_kernel_oom(fault_addr, regs);
  }
}
