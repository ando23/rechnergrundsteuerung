#pragma once
#include <stdint.h>

void check_cpu();

void cpu_disable_interrupts();

void cpu_enable_interrupts();

void cpu_call_interrupt(uint16_t intNo);

void cpu_halt();

void cpu_out8(uint16_t port, uint8_t value);

uint8_t cpu_in8(uint16_t port);

