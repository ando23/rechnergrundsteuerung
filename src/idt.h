#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "makros.h"

#define KERNEL_CS	(0x08)
#define IDT_MAX_DESCRIPTORS	256


typedef struct {
	uint16_t	isr_addr_lo;	
	uint16_t	isr_codeseg;	
	uint8_t	reserved;	
	uint8_t	flags;	
	uint16_t	isr_addr_hi;	
} STRUCT_PACKED idt_entry_32_t;

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} STRUCT_PACKED idtr_32_t;

__attribute__ ((aligned(0x10)))
idt_entry_32_t idt32[256];	// Actual IDT
static idtr_32_t idtr32;	

// Parameters for ISR
typedef struct {
	uint32_t	eip;
	uint32_t	cs;
	uint32_t	eflags;
	uint32_t	esp;
	uint32_t	ss;
} STRUCT_PACKED int_frame_32_t;


// Default exception handler
SYSA_KERNEL_NORETURN
void default_exception_handler(int_frame_32_t* frame) {
	
}

// Default exception handler
SYSA_KERNEL_NORETURN
void default_exception_handler_errcode(int_frame_32_t* frame, uint32_t error_code) {
	
}

// Default interrupt handler
SYSA_KERNEL_NORETURN
void default_interrupt_handler(int_frame_32_t* frame) {
	
}


void idt_set_descriptor_32(uint8_t entry_number, void* isr, uint8_t flags);

void init_idt();



void idt_set_descriptor_32(uint8_t entry_number, void* isr, uint8_t flags) {
	idt_entry_32_t* descriptor = &idt32[entry_number];
 
    descriptor->isr_addr_hi	= (uint32_t)isr >> 16;
    descriptor->isr_addr_lo	= (uint32_t)isr & 0xFFFF;
    descriptor->isr_codeseg	= KERNEL_CS;
    descriptor->flags	= flags;
    descriptor->reserved	= 0;

}

//extern void* isr_stub_table[];

void idt_init() {
	idtr32.base = (uintptr_t)&idt32[0];
	idtr32.limit = (uint16_t)sizeof(idt_entry_32_t) * IDT_MAX_DESCRIPTORS - 1;

/*
	for (uint8_t vector = 0; vector < 32; vector++) {
		idt_set_descriptor_32(vector, isr_stub_table[vector], 0x8E);
		//vectors[vector] = true;
	}
	__asm__ volatile ("lidt %0" : : "m"(idtr32)); // load the new IDT
	__asm__ volatile ("sti"); // set the interrupt flag
*/
}

