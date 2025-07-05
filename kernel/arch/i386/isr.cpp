#include <stdint.h>
#include <stdio.h>
#include "isr.h"

ISRTable ISRTable::instance;

extern "C"
void exception_handler(Registers* regs) {
  printf("Interrupt");
  ISRTable::instance.call(regs->interrupt, regs);
}