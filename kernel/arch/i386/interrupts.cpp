#include <kernel/kernellib.h>
#include <kernel/paging.h>
#include <kernel/physical_alloc.h>
#include <kernel/proc.h>
#include <stdio.h>

#include "interrupts.h"
#include "memory/paging_defs.h"

void handle_user_oom(uint32_t fault_addr, trapframe* regs) {
  void* phys = alloc_physical_page();
  if (phys)
    map_page(phys, (void*)fault_addr, PAGE_PRESENT | PAGE_RW | PAGE_USER);
  else
    panic("User OOM");
}

void handle_kernel_oom(uint32_t fault_addr, trapframe* regs) {
  void* phys = alloc_physical_page();
  if (phys)
    map_page(phys, (void*)fault_addr, PAGE_PRESENT | PAGE_RW);
  else
    panic("Kernel OOM");
}

void handle_page_fault(trapframe* regs) {
  uint32_t fault_addr;
  asm volatile("mov %%cr2, %0" : "=r"(fault_addr));

  uint32_t page_addr = fault_addr & ~0xFFF;  // align

  uint32_t error_code = regs->err;
  int present = error_code & 0x1;
  int write = error_code & 0x2;
  int user = error_code & 0x4;
  if (present) {
    if (user && current_process) {
      current_process->state = proc_state::Zombie;
      yield();
      return;
    }
    panic("Page Fault Protection Violation");  // TODO sys_exit if user;
  }
  if (!user) panic("Kernel page fault");  // TODO: maybe someday will go away
  void* page = alloc_physical_page();
  if (!page) {
    if (user)
      handle_user_oom(fault_addr, regs);
    else
      handle_kernel_oom(fault_addr, regs);
  }
  uint32_t flags = PAGE_PRESENT | PAGE_RW;
  if (user) flags |= PAGE_USER;
  map_page(page, (void*)fault_addr, flags);
}

// Dummy syscall handlers for init process
static int sys_exec(void) {
  // For now, just return success - init process will be replaced
  // Real implementation would load /init from disk
  return 0;
}

static int sys_exit(void) {
  if (current_process) {
    current_process->state = proc_state::Zombie;
    yield();
  }
  return 0;
}

static int sys_getpid(void) {
  if (current_process) return current_process->pid;
  return -1;
}

void syscall_handler(trapframe* regs) {
  int syscall_num = regs->eax;
  printf("interrupt=%u syscall_num=%u cs=0x%x cpl=%u\n", regs->trapno,
         syscall_num, regs->cs, regs->cs & 3);
  int result = -1;
  switch (syscall_num) {
    case 1:  // SYS_exec
      result = sys_exec();
      break;
    case 2:  // SYS_exit
      result = sys_exit();
      break;
    case 11:  // SYS_getpid (common)
      result = sys_getpid();
      break;
    default:
      result = -1;
  }
  regs->eax = result;
}
