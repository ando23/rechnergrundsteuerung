// Copyright 2022-2023; for authors see bottom
// Licence: MIT

#include <stddef.h>
#include <stdint.h>

struct GDT_Descriptors_t {
//	dw	8 * 3 - 1	; For limit storage
//	dd	GDT_Table	; For base storage
//	uint16_t limits[];
//	void* gdt_table; 
};

struct GDT_Table_entry_t {
	uint16_t limit0;	// Limit 0-15
	uint16_t base0;	// Base 0-15
	uint8_t base16;	// Base 16-23
	union {
		uint8_t access;	// Access Byte
	};
	union {
		uint8_t limit16_flags; // Limit 16-19 und 4 bit Flags
	};
	uint8_t base24;	//  Base 24-31
	
};
void gte_set(struct GDT_Table_entry_t* entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);


void memset(void *s, uint8_t c, size_t n);
void memset16(void *s, uint16_t c, size_t n);
void memcpy(void *dst, void *src, size_t n);
void memmove(void *dst, void *src, size_t n);


/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */