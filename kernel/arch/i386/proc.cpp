// kernel/arch/i386/proc.cpp
#include <kernel/kernellib.h>
#include <kernel/paging.h>
#include <kernel/physical_alloc.h>
#include <kernel/proc.h>
#include <kernel/proc_arch.h>
#include <kernel/virtual_alloc.h>
#include <stdio.h>
#include <string.h>

#include "memory/paging_defs.h"

extern "C" {
extern uint8_t _binary_initcode_start[];
extern uint8_t _binary_initcode_size;
}

extern void set_tss_esp0(uint32_t esp);

void copy_kernel_mappings(void* pgdir) {
  uint32_t* new_pgdir = (uint32_t*)pgdir;
  uint32_t* kernel_pgdir = (uint32_t*)0xFFFFF000;

  for (int i = 768; i < 1024; i++) new_pgdir[i] = kernel_pgdir[i];
  new_pgdir[1023] = (uint32_t)get_physaddr(pgdir);
}

int load_initcode(struct process* p) {
  void* phys = alloc_physical_page();
  if (!phys) return -1;

  map_page(phys, (void*)0x1000, PAGE_PRESENT | PAGE_RW | PAGE_USER);
  memcpy((void*)0x1000, _binary_initcode_start, (size_t)&_binary_initcode_size);
  void* stack_phys = alloc_physical_page();
  if (!stack_phys) {
    free_physical_page(phys);
    return -1;
  }
  map_page(stack_phys, (void*)0x2000, PAGE_PRESENT | PAGE_RW | PAGE_USER);

  p->sz = 0x3000;
  return 0;
}

void setup_process_trapframe(struct process* p, uint32_t entry,
                             uint32_t stack_top) {
  struct trapframe* tf = (struct trapframe*)p->tf;
  memset(tf, 0, sizeof(*tf));

  tf->cs = 0x1B;  // User code segment (selector 0x18 | 0x3)
  tf->ds = 0x23;  // User data segment (selector 0x20 | 0x3)
  tf->es = 0x23;
  tf->fs = 0x23;
  tf->gs = 0x23;
  tf->ss = 0x23;
  tf->eflags = 0x202;
  tf->eip = entry;
  tf->esp = stack_top;
  tf->user_esp = stack_top;

  set_tss_esp0((uint32_t)p->kstack + KERNEL_STACK_SIZE);
}

extern "C" void switchkvm(void* pgdir) {
  if (pgdir == nullptr) {
    pgdir = get_physaddr((void*)0xFFFFF000);
  }
  paging_load_directory((uint32_t*)pgdir);
}

extern "C" void forkret(void) {}
