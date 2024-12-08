// Copyright 2022-2024; for authors see bottom
// Licence: MIT

#include "memory.h"

void gte_set(GDT_segment_descriptor_t* entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	entry->base0 = (base & 0xFFFF);
	entry->base16 = (base >> 16) & 0xFF;
	entry->base24 = (base >> 24);
	
	entry->limit0 = (limit & 0xFFFF);
	//TODO check me
	entry->limit16_flags = ((limit >> 16) << 4) | (flags & 0xF);
	
	entry->access = access;
}

GDT_descriptor_table_t	gdt_table;
GDT_segment_descriptor_t	gdt[32];


void GDT_init() {
	GDT_segment_descriptor_t* p = gdt;
	
	// Tabelle initialisieren:
	gdt_table.limits = 32;
	gdt_table.addr_entries = p;
	
	for (size_t i = 0; i < gdt_table.limits; i++) {
		// so?
		gte_set(&gdt[i], 0, 0, 0, 0);
	}
	
	//NULL descriptor
	gte_set(p++, 0, 0, 0, 0);
	
	// 0x08 Code Ring 0 LINEAR_CODE_SELECTOR
	gte_set(p++, 0, 0xffffffff, 0b10011010, 0b1100);
	//gte_set(&gdt[0x08], 0, 0, 0, 0);
	
	// 0x10 Data Ring 0 LINEAR_DATA_SELECTOR
	gte_set(p++, 0, 0xffffffff, 0b10010010, 0b1100);
	//gte_set(&gdt[0x10], 0, 0, 0, 0);
	
	// Code Ring 3
	gte_set(p++, 0, 0xffffffff, 0b11111010, 0b1100);
	// Data Ring 3
	gte_set(p++, 0, 0xffffffff, 0b11110010, 0b1100);

	// Task State Segment
	gte_set(p++, 0, 0, 0, 0);
	
	// Other
	gte_set(p++, 0, 0, 0, 0);
	gte_set(p++, 0, 0, 0, 0);
	gte_set(p++, 0, 0, 0, 0);
	gte_set(p++, 0, 0, 0, 0);
	

	//asm ( "lgdt [gdt_table]"
	asm volatile (
		"	lgdt	[eax]	\r\n"
		"	jmp	0x08:reload_CS1	\r\n"
		"reload_CS1:	\r\n"
		"	mov	ax, 0x10	\r\n"
		"	mov	ds, ax	\r\n"
		"	mov	es, ax	\r\n"
		"	mov	fs, ax	\r\n"
		"	mov	gs, ax	\r\n"
		"	mov	ss, ax	\r\n"
		:
		: "a"(&gdt_table)
		: "memory"
	);
}

void reload_segments() {
	// Reload CS register containing code selector:
	// 0x08 points at the new code selector
	
	// Reload data segment registers:
	// 0x10 points at the new data selector
	
	asm volatile (
	"	jmp   0x08:reload_CS2	\r\n"
	"reload_CS2:	\r\n"
	"	mov   ax, 0x10	\r\n"
	"	mov   ds, ax	\r\n"
	"	mov   es, ax	\r\n"
	"	mov   fs, ax	\r\n"
	"	mov   gs, ax	\r\n"
	"	mov   ss, ax	\r\n"
	: : : "memory" );
}



/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */