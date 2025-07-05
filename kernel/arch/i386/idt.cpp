#include "idt.h"
#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 48

extern "C" void* isr_table[];

__attribute__((aligned(0x10)))
IDTable IDTable::instance;

void IDTable::emplace(uint8_t flags) {
  this->entries[cnt] = IDTEntry{isr_table[cnt], flags};
  cnt++;
}

struct IDTPtr {
  uint16_t limit;
  uint32_t base;
}__attribute__((packed));

IDTPtr idt_ptr{sizeof(IDTable::instance.entries) - 1,(uint32_t)&IDTable::instance.entries};

extern "C"
void IDT_configure() {
  for (uint8_t i = 0; i < IDT_MAX_DESCRIPTORS; ++i)
    IDTable::instance.emplace(0x8E);
}