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
#include "pci.h"
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
	//rtc_print();
	
	// Enter Protected Mode
	kputs("* Initializing Protected Mode\n");
	cpu_enter_protectedmode();
	
	// Init interrupts
	kputs("* Initializing interrupts\n");
	idt_init();
	cpu_enable_interrupts();		// Interrupts töten, Karl!
	//cpu_call_interrupt(49);		// Test

	// Init timer
	kputs("* Initializing Timer\n");
	pit_init();
	
	// PCI
	kputs("* Initializing PCI\n");
	pci_init();	

	// File system
	kputs("* Initializing virtual file system\n");
	//vfs_init();
	// load init fs
	// load configuration file

	// GUI
	//kputs("* Initializing video\n");
	//gfx_init();

	
	// kinit_done:
	kputs("* Initializing done\n");

/*
	kputl(0x0001);kputnl();
	kputl(0x0012);kputnl();
	kputl(0x0123);kputnl();
	kputl(0x1234);kputnl();
	kputl(0x12345);kputnl();
	kputl(0x123456);kputnl();
	kputl(0x1234567);kputnl();
	kputl_dec(0x00); kputnl();
	kputl_dec(0x01); kputnl();
	kputl_dec(0x10); kputnl();
	kputl_dec(0x12); kputnl();
	kputl_dec(0xA0); kputnl();
	kputl_dec(0xFF); kputnl();
	kputl_dec(0xFFFF); kputnl();
	kputl_dec(0xFFFFFFFF); kputnl();
*/

	kputs("* Entering kernel loop");
	//scheduler_init();
	//kernel_loop();
	//kernel_shutdown();
	kernel_demo();
}



void kernel_demo() {
	uint16_t* vmem = (uint16_t*)0xB8000L;
	uint16_t indicators[] = { 0x4041, 0x3042, 0x2043, 0x6044 };
	uint8_t indicatorCount = 4, currentIndicator = 0;
	
	while (true) {

		// bissle Grafik
		for (size_t j=0; j<50000; j++) {
			
			uint16_t indicator = indicators[currentIndicator];
			
			for (size_t i=40; i<80; i++) {
				vmem[i] = indicator;
			}

			currentIndicator++;
			if (currentIndicator >= indicatorCount)
				currentIndicator = 0;
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
	kprintf("^1Multiboot ^3flags=^7%x\n", infos->flags);
	
	if (IS_BIT_SET(infos->flags,0)) {
		kprintf("^1[0]\tmem_lower:\t^7%x\n", infos->mem_lower);
		kprintf("^1[0]\tmem_higher:\t^7%x\n", infos->mem_higher);
	}
	if (IS_BIT_SET(infos->flags,1)) {
		kprintf("^1[1]\tboot_device:\t^7%x\n", infos->boot_device);
	}
	if (IS_BIT_SET(infos->flags,2)) {
		kprintf("^1[2]\tcmdline:\t^7%x\n", infos->cmdline);
	}
	if (IS_BIT_SET(infos->flags,3)) {
		kprintf("^1[3]\tmods_count:\t^7%d\n", infos->mods_count);
		kprintf("^1[3]\tmods_addr:\t^7%x\n", infos->mods_addr);
	}
	if (IS_BIT_SET(infos->flags,4) || IS_BIT_SET(infos->flags,5)) {
		//char syms[12];			// flags[4] || flags[5]
		//kprintf("^1[4]\tsyms:\t^7%d\n", infos->mods_count);
		kputs("[4]\tsyms:\t");kputnl();
	}
	if (IS_BIT_SET(infos->flags,6)) {
		kprintf("^1[6]\tmmap_length:\t^7%d\n", infos->mmap_length);
		kprintf("^1[6]\tmmap_addr:\t^7%x\n", infos->mmap_addr);
	}
	if (IS_BIT_SET(infos->flags,7)) {
		kputs("[7]\tdrives_length:\t");kputl(infos->drives_length);kputnl();
		kputs("[7]\tdrives_addr:\t");kputl(infos->drives_addr);kputnl();
	}
	if (IS_BIT_SET(infos->flags,8)) {
		kprintf("^1[8]\tconfig_table:\t^7%x\n", infos->config_table);
	}
	if (IS_BIT_SET(infos->flags,9)) {
		kprintf("^1[9]\tboot_loader_name:\t^7%x\t^3%s\n", infos->boot_loader_name, (char*)infos->boot_loader_name);
		//kputs("[9]\tboot_loader_name:\t");kputl(infos->boot_loader_name);kputnl();
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
		kprintf("^1[12]\tfb_addr:\t^7%d\n", infos->fb_addr);
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