#pragma once
#include <kernel/proc_arch.h>
#include <stdint.h>

enum class proc_state {
  Unused,
  Embryo,
  Runnable,
  Running,
  Sleeping,
  Zombie,
};

#define KERNEL_STACK_SIZE 4096
#define MAX_PROC_AMOUNT 64

struct process {
  uint32_t pid;
  proc_state state;
  struct trapframe* tf;     // Platform-specific trap frame
  struct context* context;  // Platform-specific context
  void* kstack;
  void* pgdir;
  uint32_t sz;
  char name[32];
};

extern struct process proc_table[MAX_PROC_AMOUNT];
extern struct process* current_process;
extern struct process* initproc;

// Platform-independent functions
void init_processes();
struct process* alloc_process();
void userinit();  // Creates first process
void scheduler();
void sched();
void yield();

// Platform-dependent functions (implemented in arch/)
extern "C" {
void swtch(struct context** old, struct context* n);
void forkret(void);
void switchkvm(void* pgdir);
void trapret(void) __attribute__((noreturn));
}
// Platform-independent functions that call platform-specific implementations
void copy_kernel_mappings(void* pgdir);
int load_initcode(struct process* p);
void setup_process_trapframe(struct process* p, uint32_t entry,
                             uint32_t stack_top);
