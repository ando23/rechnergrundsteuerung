// Copyright 2022-2024; for authors see bottom
// Licence: MIT

// Info: https://wiki.osdev.org/PIT

#include "pit.h"
#include "cpu.h"
#include "string.h"
#include "serial.h"

#define PIT_CHN0	0x40
#define PIT_CHN1	0x41
#define PIT_CHN2	0x42
#define PIT_CMD	0x43  

#define PIT_OS_FLAGS	0b00010110
//	  xx......	channel
//	  ..xx....	access mode, 01=lobyte only
//	  ....xxx.	operating mode, 3=square wave
//	  .......0	0=binary 1=BCD


volatile uint32_t pit_counter;


void pit_init() {
	pit_counter = 0;

	//pushad
	//pushfd
	//cpu_disable_interrupts();
	
	//cpu_out8( PIT_CMD, PIT_OS_FLAGS );
	
	//cpu_enable_interrupts();
	//popfd
	//popad
}

/*
ISR_pit:
	push eax
	push ebx
	
	; do something
	mov eax, [pit_counter]
	inc eax
	mov [pit_counter], eax

	mov al, 0x20
	out 0x20, al	; Send the EOI to the PIC

	pop ebx
	pop eax
	iretd
*/

void pit_print() {
	char buf[30];
	snprintf(buf, sizeof(buf), "PIT: Zähler=%d\r\n", pit_counter);
	kputs(buf);
}

