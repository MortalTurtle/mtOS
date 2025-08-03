#pragma once
#include <stdint.h>

void PIC_sendEOI(uint8_t irq);

void PIC_remap(uint8_t offset1, uint8_t offset2);

void PIC_disable();

void IRQ_set_mask(uint8_t IRQ_line);

void IRQ_clear_mask(uint8_t IRQ_line);

uint16_t PIC_read_isr_reg();
uint16_t PIC_read_irq_reg();

extern "C"
void PIC_configure();