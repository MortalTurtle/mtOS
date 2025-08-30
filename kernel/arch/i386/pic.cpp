#include "pic.h"
#include <stdint.h>
#include "inline_asm.h"
#include "interrupts.h"
#include "isr.h"
#include "kernel/keyboard.h"
#include "stdio.h"

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20
#define PIC_READ_IRR 0x0A
#define PIC_READ_ISR 0x0B

void PIC_sendEOI(uint8_t irq) {
  if (irq >= 8) outb(PIC2_COMMAND, PIC_EOI);
  outb(PIC1_COMMAND, PIC_EOI);
}

#define ICW1_ICW4 0x01      // ICW4 will be present
#define ICW1_SINGLE 0x02    // Single (cascade) mode
#define ICW1_INTERVAL 0x04  // Call address interval 4(8)
#define ICW1_LEVEl 0x08     // Level triggered (edge) mode
#define ICW1_INIT 0x10      // Initialization - required!

#define ICW4_8086 0x01        // 8086/88 (MCS-80/55) mode
#define ICW4_AUTO 0x02        // Auto (normal) EOI
#define ICW4_BUF_SLAVE 0x08   // buffered mode/slave
#define ICW4_BUF_MASTeR 0x0C  // buffered mode/master
#define ICW4_SFNM 0x10        // special fully nested(not)

void PIC_remap(uint8_t offset1, uint8_t offset2) {
  // start init
  outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
  io_wait();
  outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
  io_wait();
  // set vector offset
  outb(PIC1_DATA, offset1);
  io_wait();
  outb(PIC2_DATA, offset2);
  io_wait();
  // tell master there is a slave, tell slave its cascade identity
  outb(PIC1_DATA, 0x4);  // PIC1 has slave at IRQ2
  io_wait();
  outb(PIC2_DATA, 0x2);  // PIC2 cascade mode
  io_wait();
  // use 8086 mode
  outb(PIC1_DATA, ICW4_8086);
  io_wait();
  outb(PIC2_DATA, ICW4_8086);
  io_wait();
  // unmask
  outb(PIC1_DATA, 0);
  outb(PIC2_DATA, 0);
}

void PIC_disable() {
  outb(PIC1_DATA, 0xff);
  outb(PIC2_DATA, 0xff);
}

void IRQ_set_mask(uint8_t IRQ_line) {
  uint16_t port;
  uint8_t val;
  if (IRQ_line < 8)
    port = PIC1_DATA;
  else {
    port = PIC2_DATA;
    IRQ_line -= 8;
  }
  val = inb(port) | (1 << IRQ_line);
  outb(port, val);
}

void IRQ_clear_mask(uint8_t IRQ_line) {
  uint16_t port;
  uint8_t val;
  if (IRQ_line < 8)
    port = PIC1_DATA;
  else {
    port = PIC2_DATA;
    IRQ_line -= 8;
  }
  val = inb(port) & ~(1 << IRQ_line);
  outb(port, val);
}

uint16_t PIC_read_irq_reg() {
  outb(PIC1_COMMAND, PIC_READ_IRR);
  outb(PIC2_COMMAND, PIC_READ_IRR);
  return (inb(PIC1_COMMAND) | inb(PIC2_COMMAND) << 8);
}

uint16_t PIC_read_isr_reg() {
  outb(PIC1_COMMAND, PIC_READ_ISR);
  outb(PIC2_COMMAND, PIC_READ_ISR);
  return (inb(PIC1_COMMAND) | inb(PIC2_COMMAND) << 8);
}

#define PIC_REMAP_OFFSET 0x20

void irq_stub_handler(Registers* regs) {
  printf("Irq interrupt %d", (regs->interrupt - PIC_REMAP_OFFSET));
  PIC_sendEOI(regs->interrupt - PIC_REMAP_OFFSET);
}

void irq0_timer_handler(Registers* regs) {
  PIC_sendEOI(regs->interrupt - PIC_REMAP_OFFSET);
}

void irq1_keyboard_handler(Registers* regs) {
  uint8_t scancode = inb(0x60);
  if (!(scancode & 0x80)) {  // proccess only presses
    char c = scancode_to_ascii(scancode);
    if (c) {
      // TODO do better keyboard interrupt
      putchar(c);
    }
  }
  PIC_sendEOI(regs->interrupt - PIC_REMAP_OFFSET);
}

extern "C" void PIC_configure() {
  PIC_remap(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);
  ISRTable::instance.isr_table[PIC_REMAP_OFFSET] = irq0_timer_handler;
  ISRTable::instance.isr_table[PIC_REMAP_OFFSET + 1] = irq1_keyboard_handler;
  ISRTable::instance.valid[PIC_REMAP_OFFSET] = true;
  ISRTable::instance.valid[PIC_REMAP_OFFSET + 1] = true;
  for (int i = 2; i < 16; i++) {
    ISRTable::instance.isr_table[PIC_REMAP_OFFSET + i] = irq_stub_handler;
    ISRTable::instance.valid[PIC_REMAP_OFFSET + i] = true;
  }
}
