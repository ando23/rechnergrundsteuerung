#pragma once
#include <stdint.h>

void cpu_disable_interrupts() {
	asm("cli");
}

void cpu_enable_interrupts() {
	asm("sti");
}

void cpu_call_interrupt(uint16_t intNo) {
	asm("int 49");
}

void cpu_halt() {
	asm("hlt");
}

void cpu_out8(uint16_t port, uint8_t value) {
	/*
	mov dx, 0x3D4
	mov al, 0xA	; 
	out dx, al
	*/
}

uint8_t cpu_in8(uint16_t port) {
	return 0;
}
