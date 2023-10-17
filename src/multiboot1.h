
#pragma once

#include <stdint.h>

#define MB1_MAGIC			0x1BADB002
#define MB1_MAGIC_RESP		0x2BADB002

#define MB1_ALIGN			(1 << 0)
#define MB1_MEMORY			(1 << 1)
#define MB1_AOUT_KLUDGE		(1 << 16)
#define MB1_FLAGS			(MB1_ALIGN | MB1_MEMORY | MB1_AOUT_KLUDGE)
#define MB1_CHKSUM			-(MB1_MAGIC + MB1_FLAGS)

//MB1_FLAGS2	equ	0b1000000000000011
//	;	  ...............x	ALIGN
//	;	  ..............x.	MEMORY
//	;	  x...............	AOUT_KLUDGE

struct multiboot1_header {
	uint32_t magic;
	uint32_t flags;
	uint32_t checksum;
	
	// Addr bei AOUT_KLUDGE
	uint32_t multiboot1_header;	// header addr
	uint32_t start_of_kernel;	// load to
	uint32_t _ignored0;
	uint32_t _ignored1;
	uint32_t kernel_start;	// starting point

	// Grub VGA
	uint32_t _vga0;
	uint32_t _vga1;
	uint32_t _vga2;
	uint32_t _vga3;
};

struct multiboot1_infoptr {
	uint32_t magic;
};

