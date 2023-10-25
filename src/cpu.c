
#include "cpu.h"


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


void check_cpu() {
	/*
	// Hersteller
	mov eax, 0
	cpuid			// eax hat hier höchstmöglichen Wert für CPUID
	
	mov [cpuinfo + 0], ebx
	mov [cpuinfo + 4], edx
	mov [cpuinfo + 8], ecx
	mov eax, cpuinfo1
	call kputs

	// Features
	mov eax, 1
	cpuid
	push eax
	mov eax, cpuinfo2
	call kputs
	pop eax
	call kputl
	mov eax, cpuinfo3
	call kputs
	mov eax, edx
	call kputl
	mov eax, cpuinfo4
	call kputs
	mov eax, ecx
	call kputl
	mov eax, cpuinfonl
	call kputs
	*/
}



void enter_pmode() {
	enable_A20();
	init_GDT();
	
	/*
	; enable protected mode
	mov	eax, cr0
	or	al, 1
	mov	cr0, eax

	jmp	08h:kernel_pmode	; set CS
kernel_pmode:
	*/
	reload_segments();	// set other segments
}

void enable_A20() {
	/*
	in	al, 0x92
	or	al, 2
	out	0x92, al
	*/
}

void init_sse() {
	/*
	mov	eax, cr0
	and	ax, 0b1111_1101
	or	ax, 0b0000_0001
	mov	cr0, eax
	
	mov	eax, cr4
	or	ax, 0b11_0000_0000
	mov	cr4, eax
	*/
}