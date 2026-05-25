#include <kernel/kernellib.h>
#include <kernel/proc.h>

#include "kernel/memory.h"
#include "kernel/physical_alloc.h"
#include "kernel/virtual_alloc.h"

extern void set_tss_esp0(uint32_t esp);

struct context* scheduler_context = nullptr;

void scheduler() {
  struct process* p;
  for (;;) {
    asm volatile("sti");
    for (p = proc_table; p < &proc_table[MAX_PROC_AMOUNT]; p++) {
      if (p->state != proc_state::Runnable) continue;
      current_process = p;
      set_tss_esp0((uint32_t)p->kstack + KERNEL_STACK_SIZE);
      switchkvm(p->pgdir);
      p->state = proc_state::Running;
      swtch(&scheduler_context, p->context);
      switchkvm(nullptr);
      if (p->state == proc_state::Zombie) {
        p->state = proc_state::Unused;
        if (p->kstack) kvfree(p->kstack, KERNEL_STACK_SIZE / 4096);
        if (p->pgdir) free_physical_page(p->pgdir);
      }
    }
  }
}

void sched() {
  struct context* old_ctx = current_process->context;
  asm volatile("cli");
  swtch(&old_ctx, scheduler_context);
  current_process->context = old_ctx;
  asm volatile("sti");
}

void yield() {
  asm volatile("cli");
  if (current_process && current_process->state == proc_state::Running) {
    current_process->state = proc_state::Runnable;
    sched();
  }
  asm volatile("sti");
}
