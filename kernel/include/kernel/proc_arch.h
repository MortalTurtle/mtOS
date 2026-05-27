#pragma once
#include <stdint.h>

#ifdef i386
struct trapframe {
  // pusha
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t oesp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;

  // segment registers
  uint32_t gs;
  uint32_t fs;
  uint32_t es;
  uint32_t ds;

  // pushed manually
  uint32_t trapno;
  uint32_t err;

  // pushed by CPU
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;

  // only on privilege change
  uint32_t esp;
  uint32_t ss;
};

struct context {
  uint32_t edi;
  uint32_t esi;
  uint32_t ebx;
  uint32_t ebp;
  uint32_t eip;
};
#endif
