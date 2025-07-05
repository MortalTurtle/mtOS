#pragma once
#include <stdint.h>

struct IDTEntry {
  uint16_t isr_low;
  uint16_t selector;
  uint8_t reserved;
  uint8_t attributes;
  uint16_t isr_high;

  IDTEntry() {}
  IDTEntry(void* isr, uint8_t flags) :
    isr_low((uint32_t)isr & 0xFFFF),
    selector(0x08),
    reserved(0),
    attributes(flags),
    isr_high(((uint32_t)isr) >> 16) {}
}__attribute__((packed));

struct IDTable {
  IDTEntry entries[256];
  static IDTable instance;
  void append(const IDTEntry& entry) {
    entries[cnt++] = entry;
  }
  void emplace(uint8_t flags);
  private:
    int cnt = 0;
    IDTable() {}
    IDTable(IDTable&) = delete;
    IDTable(IDTable&&) = delete;
}__attribute__((packed));