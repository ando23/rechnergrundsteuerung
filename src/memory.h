// Copyright 2022-2023; for authors see bottom
// Licence: MIT

#include <stddef.h>
#include <stdint.h>
#include "makros.h"

#define GDT_ENTRY_MAX_COUNT	3

typedef struct {
//	dw	8 * 3 - 1	; For limit storage
//	dd	GDT_Table	; For base storage
	uint16_t limits;
	void* addr_entries;
} STRUCT_PACKED GDT_descriptor_table_t;

typedef struct {
	uint16_t limit0;	// Limit 0-15
	uint16_t base0;	// Base 0-15
	uint8_t base16;	// Base 16-23
	union {
		uint8_t access;	// Access Byte
		struct {
			uint8_t type:4;
			uint8_t S:1;
			uint8_t DPL:2;
			uint8_t P:1;
		};
	};
	union {
		uint8_t limit16_flags; // Limit 16-19 und 4 bit Flags
	};
	uint8_t base24;	//  Base 24-31

} STRUCT_PACKED GDT_segment_descriptor_t;

void gte_set(GDT_segment_descriptor_t* entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

void GDT_init();
void reload_segments();

/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */