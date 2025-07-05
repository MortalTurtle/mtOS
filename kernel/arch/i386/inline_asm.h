#pragma once
#include <stdint.h>


static inline void outb(uint16_t port, uint8_t val) {
  __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile ("inb %w1, %b0"
                  : "=a"(ret)
                  : "Nd"(port)
                  : "memory");
  return ret;
}

static inline void io_wait(void) {
  outb(0x80, 0);
}

static inline bool are_interrupts_enabled() {
  unsigned long flags;
  __asm__ volatile ("pushf\n\t"
                    "pop %0"
                    : "=g"(flags));
  return flags & (1 << 9);
}

static inline void invlpg(void* m) {
  __asm__ volatile ("invlpg (%0)" : : "b"(m) : "memory");
}
