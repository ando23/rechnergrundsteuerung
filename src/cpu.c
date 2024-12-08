#include "cpu.h"
#include "memory.h"
#include "string.h"

// https://wiki.osdev.org/Inline_Assembly

void cpu_disable_interrupts() {
	//asm(".intel_syntax noprefix");	// MUST BE FIRST IN FILE
	asm volatile ("cli" : : : "memory");
}

void cpu_enable_interrupts() {
	asm volatile ("sti");
}

void cpu_call_interrupt(uint16_t intNo) {
	asm volatile (
		"int 49" 	// WARUM nicht 'al'?
		:
		: "a"(intNo)
		: "memory"
	);
}

void cpu_halt() {
	asm volatile ("hlt");
}

void cpu_out8(uint16_t port, uint8_t value) {
	asm volatile (
		"out dx, al"
		:
		: "d"(port), "a"(value)
		: 
	);
}
void cpu_out16(uint16_t port, uint16_t value) {
	asm volatile (
		"out dx, ax"
		:
		: "d"(port), "a"(value)
		: 
	);
}
void cpu_out32(uint16_t port, uint32_t value) {
	asm volatile (
		"out dx, eax"
		:
		: "d"(port), "a"(value)
		: 
	);
}

uint8_t cpu_in8(uint16_t port) {
	uint8_t result;
	asm volatile (
		"in al, dx"
		: "=a"(result)
		: "d"(port)
		: 
	);
	return result;
}

uint16_t cpu_in16(uint16_t port) {
	uint16_t result;
	asm volatile (
		"in ax, dx"
		: "=a"(result)
		: "d"(port)
		:
	);
	return result;
}

uint32_t cpu_in32(uint16_t port) {
	uint32_t result;
	asm volatile (
		"in eax, dx"
		: "=a"(result)
		: "d"(port)
		:
	);
	return result;
}

	
void cpu_enable_A20() {
	const uint8_t v = cpu_in8(0x92) | 2;
	cpu_out8(0x92, v);
}
	
void cpu_init_sse() {
	asm volatile ( 
		"mov	eax, cr0	\r\n"
		"and	ax, 0b11111101	\r\n"
		"or	ax, 0b00000001	\r\n"
		"mov	cr0, eax	\r\n"
		"mov	eax, cr4	\r\n"
		"or	ax, 0b1100000000	\r\n"
		"mov	cr4, eax	\r\n"
	);
}

void cpu_enter_protectedmode() {
	cpu_enable_A20();
	GDT_init();

	// enable protected mode
	asm (
		"mov	eax, cr0	\r\n"
		"or	al, 1	\r\n"
		"mov	cr0, eax	\r\n"

		"jmp	0x08:kernel_pmode	\r\n"	// set CS
		"kernel_pmode:	\r\n"
		: : : "memory"
	);
	reload_segments();	// set other segments
}






/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */