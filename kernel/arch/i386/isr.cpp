#include <stdint.h>

#include "isr.h"

ISRTable ISRTable::instance;

extern "C" void exception_handler(trapframe* regs) {
  ISRTable::instance.call(regs->trapno, regs);
}
