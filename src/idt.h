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


// Parameters for ISR
typedef struct {
	uint32_t	eip;
	uint32_t	cs;
	uint32_t	eflags;
	uint32_t	esp;
	uint32_t	ss;
} STRUCT_PACKED int_frame_32_t;




void idt_set_descriptor_32(uint8_t entry_number, void* isr, uint8_t flags);

void init_idt();
