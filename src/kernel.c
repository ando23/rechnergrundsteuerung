// Copyright 2022-2023; for authors see bottom
// Licence: MIT

#include <stdbool.h>
#include <stdint.h>
#include "cpu.h"
#include "multiboot1.h"
#include "serial.h"
#include "textmode.h"

void kernel_loop();
void kernel_shutdown();
void kernel_halt();


void kernel_main(uint32_t mbresponse, struct multiboot1_infoptr* infos) {
	// Check Multiboot
	if (mbresponse != MB1_MAGIC_RESP) {
		kernel_fail();
	}

	//mov [multiboot1_infoptr], ebx

	// OK, we have Multiboot!
	disable_cursor();
	clrscr();
	puts("msg_hello");
	set_color(0x07);

	// Init serial interface
	init_kcom();
	kputs("msg_ser");

	//kinit_cpu:
	kputs("msg_cpu");
	check_cpu();
	init_sse();

	//kinit_rtc:
	kputs("msg_init_rtc");
	init_rtc();
	rtc_update();
	rtc_print();

	// Enter Protected Mode
	kputs("msg_pmode");
	enter_pmode();

	// Init timer
	kputs("msg_pit");
	init_pit();
	
	// Init interrupts
	kputs("msg_ints");
	init_IDT();

	// kinit_done:
	kputs("msg_init_done");
	
	// Interrupts töten, Karl!
	cpu_enable_interrupts(); //	sti
	
	// Einen Software-Interrupt auslösen:
	//cpu_call_interrupt(49);

	// Scheduler
	//init_scheduler();

	kernel_loop();
	//kernel_shutdown();
	//kernel_demo();
}

/*
void kernel_demo() {
	mov edi, 0xB8000 ;+ 20 * 160
	mov bx, 0x4041
	mov cx, 0x3042
	mov eax, 0
	mov edx, 0
	
	.loop1:
		mov word [edi + eax], bx
		add ax, 2
		cmp ax, 160
		jne .loop1
		mov ax, 0
	
	.loop2:
		mov word [edi + eax], cx
		add ax, 2
		cmp ax, 160
		jne .loop2
		mov ax, 0

	inc edx
	cmp edx, 100000
	jl .loop1
	
	xor edx, edx
	call rtc_update
	call rtc_print2
	
	call pit_print
	
	jmp .loop1
}
*/

volatile bool kernel_running = true;

void kernel_loop() {
	while (kernel_running) {
		cpu_halt();
	}
	kernel_shutdown();
}

void kernel_shutdown() {
	kputs("msg_end");
	kernel_halt();
}


void kernel_halt() {
	cpu_disable_interrupts();
	while (1) {
		cpu_halt();
	}
}


/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */