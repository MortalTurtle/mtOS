#include <kernel/kernellib.h>
#include <kernel/proc.h>
#include <string.h>

#include "kernel/paging.h"
#include "kernel/physical_alloc.h"
#include "kernel/virtual_alloc.h"

struct process proc_table[MAX_PROC_AMOUNT];
struct process* current_process = nullptr;
struct process* initproc = nullptr;

void init_processes() {
  for (int i = 0; i < MAX_PROC_AMOUNT; i++) {
    proc_table[i].state = proc_state::Unused;
    proc_table[i].pid = 0;
    proc_table[i].tf = nullptr;
    proc_table[i].context = nullptr;
    proc_table[i].kstack = nullptr;
    proc_table[i].pgdir = nullptr;
    proc_table[i].sz = 0;
    proc_table[i].name[0] = '\0';
  }
  current_process = nullptr;
  initproc = nullptr;
}

struct process* alloc_process() {
  struct process* p;

  for (p = proc_table; p < &proc_table[MAX_PROC_AMOUNT]; p++)
    if (p->state == proc_state::Unused) break;
  if (p >= &proc_table[MAX_PROC_AMOUNT]) return nullptr;

  p->pid = (p - proc_table) + 1;
  p->state = proc_state::Embryo;
  p->kstack = kvalloc_immediate(KERNEL_STACK_SIZE / 4096);
  if (!p->kstack) {
    p->state = proc_state::Unused;
    return nullptr;
  }

  char* sp = (char*)p->kstack + KERNEL_STACK_SIZE;
  sp -= sizeof(trapframe);
  p->tf = (trapframe*)sp;
  sp -= 4;

  *(uint32_t*)sp = (uint32_t)trapret;

  sp -= sizeof(context);
  p->context = (context*)sp;
  memset(p->context, 0, sizeof(context));

  p->context->eip = (uint32_t)forkret;

  return p;
}

void userinit() {
  process* p = alloc_process();
  if (!p) panic("Failed to allocate init process");
  void* pgdir_virt = kvalloc_immediate(1);
  if (!pgdir_virt) panic("Failed to allocate page directory");
  p->pgdir = get_physaddr(pgdir_virt);
  memset(pgdir_virt, 0, 4096);
  copy_kernel_mappings(pgdir_virt);
  if (load_initcode(p) < 0) panic("Failed to load initcode");
  setup_process_trapframe(p, 0x1000, 0x3000);
  p->state = proc_state::Runnable;
  initproc = p;
}
