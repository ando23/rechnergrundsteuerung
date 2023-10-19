
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
	uint32_t _vga_mode;
	uint32_t _vga_width;
	uint32_t _vga_height;
	uint32_t _vga_depth;
};

struct multiboot1_infoptr {
	uint32_t flags;
	
	uint32_t mem_lower;		// requires flags[0]
	uint32_t mem_higher;	// requires flags[0]
	
	uint32_t boot_device;	// requires flags[1]
	
	uint32_t cmdline;		// requires flags[2]
	
	uint32_t mods_count;	// requires flags[3]
	uint32_t mods_addr;		// requires flags[3]
	
	char syms[12];			// flags[4] || flags[5]
	
	uint32_t mmap_length;	// requires flags[6]
	uint32_t mmap_addr;		// requires flags[6]
	
	uint32_t drives_length;	// requires flags[7]
	uint32_t drives_addr;	// requires flags[7]
	
	uint32_t config_table;	// requires flags[8]
	
	uint32_t boot_loader_name;	// requires flags[9]
	
	uint32_t apm_table;		// requires flags[10]
	
	uint32_t vbe_ctrl_info;	// requires flags[11]
	uint32_t vbe_mode_info;	//
	uint32_t vbe_mode;		//
	uint32_t vbe_if_seg;	//
	uint32_t vbe_if_off;	//
	uint32_t vbe_if_len;	//
	
	uint64_t fb_addr;		// requires flags[12]
	uint32_t fb_pitch;		//
	uint32_t fb_width;		//
	uint32_t fb_height;		//
	uint8_t fb_bpp;		//
	uint8_t fb_type;		//
	uint8_t fb_color_info[5];	//TODO in union umbauen
};

