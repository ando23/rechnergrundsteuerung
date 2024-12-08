
#include "idt.h"

__attribute__ ((aligned(0x10)))
idt_entry_32_t idt32[256];	// Actual IDT
static idtr_32_t idtr32;	


// Default exception handler
SYSA_KERNEL_NORETURN
void default_exception_handler(int_frame_32_t* frame) {
	frame = frame;
}

// Default exception handler
SYSA_KERNEL_NORETURN
void default_exception_handler_errcode(int_frame_32_t* frame, uint32_t error_code) {
	frame = frame;
	error_code = error_code;
}

// Default interrupt handler
SYSA_KERNEL_NORETURN
void default_interrupt_handler(int_frame_32_t* frame) {
	frame = frame;
}



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

	for (uint8_t vector = 0; vector < 32; vector++) {
		idt_set_descriptor_32(vector, default_interrupt_handler, 0x8E);
	}

	__asm__ volatile (
		"lidt %0	\r\n"			// load the new IDT
		"sti		\r\n"			// set the interrupt flag
		: : "m"(idtr32)
	); 

}
