// Copyright 2022-2024; for authors see bottom
// Licence: MIT

#include <stdbool.h>
#include <stdint.h>
#include "makros.h"

#include "cpu.h"
#include "cpuid.h"
#include "idt.h"
#include "memory.h"
#include "multiboot1.h"
#include "pit.h"
#include "rtc.h"
#include "serial.h"
#include "textmode.h"
#include "gfx.h"

void kernel_demo();
void kernel_loop();
void kernel_shutdown();
void kernel_halt();
void kernel_fail();

void print_cpuinfo(multiboot1_infoptr* infos);

struct kernel_vars {
	volatile bool kernel_running;
} M;

cpu_info_t cpu_info;

//SYSA_KERNEL_NORETURN
void kernel_main(uint32_t mbresponse, multiboot1_infoptr* infos) {
	disable_cursor();
	clrscr();
	set_color(0x1F);
	puts("/SYSA/");
	set_color(0x07);
	//clrscr();
	
	set_cursor_position(0,4);
	
	M.kernel_running = true;
	
	// Check Multiboot
	if (mbresponse != MB1_MAGIC_RESP) {
		puts("/kein Multiboot/");
		kernel_fail();
	}

	// Init serial interface
	init_kcom();
	kputs("* Initializing serial port\n");
	
	print_cpuinfo(infos);


	//kinit_cpu:
	kputs("* Initializing CPU\n");
	check_features(&cpu_info);
	cpu_init_sse();

	//kinit_rtc:
	kputs("* Initializing RTC\n");
	rtc_init();
	rtc_update();
	rtc_print();
	
	// Enter Protected Mode
	kputs("* Initializing Protected Mode\n");
	cpu_enter_protectedmode();

	// Init timer
	kputs("* Initializing Timer\n");
	pit_init();
	
	// Init interrupts
	kputs("* Initializing interrupts\n");
	//idt_init();

	// kinit_done:
	kputs("* Initializing done\n");
	
	// Interrupts töten, Karl!
	//cpu_enable_interrupts(); //	sti
	
	// Einen Software-Interrupt auslösen:
	//cpu_call_interrupt(49);

	// Scheduler
	//init_scheduler();
	
	//gfx_init();
	//gfx_clrscr(4);
	/*
	if (BgaIsAvailable()) {
		kputs("gfx: bochs!\n");
	}
	else {
		kputs("gfx: no bochs\n");
	}
	*/

	puts("Entering kernel loop");

	//kernel_loop();
	//kernel_shutdown();
	kernel_demo();
}


void kernel_demo() {
	uint16_t* vmem = (uint16_t*)0xB8000L;
	
	while (true) {

		// bissle Grafik
		for (size_t j=0; j<50000; j++) {
			for (size_t i=40; i<80; i++) {
				vmem[i] = 0x4041;
			}
			//sleep();
			for (size_t i=40; i<80; i++) {
				vmem[i] = 0x3042;
			}
			//sleep();
		}
	
		rtc_update();
		rtc_print();
		
		//pit_print();
	}
}

void kernel_loop() {
	while (M.kernel_running) {
		cpu_halt();
	}
	kernel_shutdown();
}

void kernel_shutdown() {
	kputs("Fahre herunter.");
	kernel_halt();
}

void kernel_fail() {
	kputs("BSOD");
	kernel_halt();
}


void kernel_halt() {
	cpu_disable_interrupts();
	while (1) {
		cpu_halt();
	}
}

void print_cpuinfo(multiboot1_infoptr* infos) {
// Multiboot info
	kputs("Multiboot flags=");
	kputl(infos->flags);
	kputnl();
	
	if (IS_BIT_SET(infos->flags,0)) {
		kputs("[0]\tmem_lower:\t");kputl(infos->mem_lower);kputnl();
		kputs("[0]\tmem_higher:\t");kputl(infos->mem_higher);kputnl();
	}
	if (IS_BIT_SET(infos->flags,1)) {
		kputs("[1]\tboot_device:\t");kputl(infos->boot_device);kputnl();
	}
	if (IS_BIT_SET(infos->flags,2)) {
		kputs("[2]\tcmdline:\t");kputl(infos->cmdline);kputnl();
	}
	if (IS_BIT_SET(infos->flags,3)) {
		kputs("[3]\tmods_count:\t");kputl(infos->mods_count);kputnl();
		kputs("[3]\tmods_addr:\t");kputl(infos->mods_addr);kputnl();
	}
	if (IS_BIT_SET(infos->flags,4) || IS_BIT_SET(infos->flags,5)) {
		//char syms[12];			// flags[4] || flags[5]
		kputs("[4]\syms:\t");kputnl();
	}
	if (IS_BIT_SET(infos->flags,6)) {
		kputs("[6]\tmmap_length:\t");kputl(infos->mmap_length);kputnl();
		kputs("[6]\tmmap_addr:\t");kputl(infos->mmap_addr);kputnl();
	}
	if (IS_BIT_SET(infos->flags,7)) {
		kputs("[7]\tdrives_length:\t");kputl(infos->drives_length);kputnl();
		kputs("[7]\tdrives_addr:\t");kputl(infos->drives_addr);kputnl();
	}
	if (IS_BIT_SET(infos->flags,8)) {
		kputs("[8]\tconfig_table:\t");kputl(infos->config_table);kputnl();
	}
	if (IS_BIT_SET(infos->flags,9)) {
		kputs("[9]\tboot_loader_name:\t");kputl(infos->boot_loader_name);kputnl();
	}
	if (IS_BIT_SET(infos->flags,10)) {
		kputs("[10]\tapm_table:\t");kputl(infos->apm_table);kputnl();
	}
	if (IS_BIT_SET(infos->flags,11)) {
		kputs("[11]\tvbe_ctrl_info:\t");kputl(infos->vbe_ctrl_info);kputnl();
		//uint32_t vbe_mode_info;	//
		//uint32_t vbe_mode;		//
		//uint32_t vbe_if_seg;	//
		//uint32_t vbe_if_off;	//
		//uint32_t vbe_if_len;	//
	}
	if (IS_BIT_SET(infos->flags,12)) {
		kputs("[12]\tfb_addr:\t");kputl(infos->fb_addr);kputnl();
		//uint64_t fb_addr;		// requires flags[12]
		//uint32_t fb_pitch;		//
		//uint32_t fb_width;		//
		//uint32_t fb_height;		//
		//uint8_t fb_bpp;		//
		//uint8_t fb_type;		//
		//uint8_t fb_color_info[5];	//TODO in union umbauen
	}
}


/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */