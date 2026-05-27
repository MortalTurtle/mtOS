#pragma once
#include <kernel/proc_arch.h>
#include <stdint.h>

void handle_page_fault(trapframe* regs);

void syscall_handler(trapframe* regs);
