#pragma once
#include <stdint.h>

void cpu_disable_interrupts();
void cpu_enable_interrupts();

void cpu_call_interrupt(uint16_t intNo);

void cpu_halt();

void cpu_out8(uint16_t port, uint8_t value);
void cpu_out16(uint16_t port, uint16_t value);

uint8_t cpu_in8(uint16_t port);
uint16_t cpu_in16(uint16_t port);


void cpu_enable_A20();
void cpu_init_sse();
void cpu_enter_protectedmode();

void init_IDT();
