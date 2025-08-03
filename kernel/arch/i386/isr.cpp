#include <stdint.h>
#include "isr.h"

ISRTable ISRTable::instance;

extern "C"
void exception_handler(Registers* regs) {
  ISRTable::instance.call(regs->interrupt, regs);
}