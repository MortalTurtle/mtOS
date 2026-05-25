#pragma once
#include <stdint.h>

#ifdef i386
struct trapframe {
  uint32_t gs;
  uint32_t fs;
  uint32_t es;
  uint32_t ds;
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
  uint32_t interrupt;
  uint32_t error;
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
  uint32_t user_esp;
  uint32_t ss;
} __attribute__((packed));

// x86-specific context (matching swtch.s expectations)
struct context {
  uint32_t edi;
  uint32_t esi;
  uint32_t ebx;
  uint32_t ebp;
  uint32_t eip;
};
#endif
