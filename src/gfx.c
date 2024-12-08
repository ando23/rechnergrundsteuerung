// Copyright 2022-2023; for authors see bottom
// Licence: MIT

#include <stddef.h>
#include <stdint.h>
#include "cpu.h"
#include "gfx.h"
#include "gfx_vbe.h"
#include "serial.h"
#include "string.h"

uint8_t* VMEM;
#define VGA_WIDTH	640
#define VGA_STRIDE	640
#define VGA_HEIGHT	480
#define VGA_BYTES	3


void gfx_init() {
	VMEM = (uint8_t*)NULL;
	
	if (BgaIsAvailable()) {
		kputs("gfx: bochs!\n");
		BgaSetVideoMode( VGA_WIDTH, VGA_HEIGHT, VBE_DISPI_BPP_24, true, true );
		//BgaSetVideoMode( 1024, 768, VBE_DISPI_BPP_24, true, true );
		
		VMEM = BgaGetFBAddr();
		
		bochs_demo();
	}
	else {
		kputs("gfx: no bochs\n");
	}
}

void gfx_clrscr(uint32_t col) {
	memset( VMEM, col, VGA_STRIDE * VGA_HEIGHT * VGA_BYTES);
}

void gfx_put_pixel(uint16_t x, uint16_t y, uint32_t col) {
	VMEM[ y * VGA_STRIDE + x ] = col;
}

void gfx_hline(uint16_t x, uint16_t y, uint16_t w, uint32_t col) {
	uint8_t* v = VMEM + VGA_STRIDE * y * VGA_BYTES + x * VGA_BYTES;
	uint8_t r = col >> 16;
	uint8_t g = col >> 8;
	uint8_t b = col & 0xff;
	for (uint16_t i = 0; i < w; i++) {
		*v++ = r;
		*v++ = g;
		*v++ = b;
	}
}
void gfx_vline(uint16_t x, uint16_t y, uint16_t h, uint32_t col) {
	uint8_t* v = VMEM + VGA_STRIDE * y * VGA_BYTES + x * VGA_BYTES;
	uint8_t r = col >> 16;
	uint8_t g = col >> 8;
	uint8_t b = col & 0xff;
	for (uint16_t i = 0; i < h; i++) {
		v[0] = r;
		v[1] = g;
		v[2] = b;
		v += VGA_STRIDE * VGA_BYTES;
	}
}


/*
void gfx_line(uint16_t x, uint16_t y, uint16_t nx, uint16_t ny, uint8_t col) {
}
*/
void gfx_rect(uint16_t x, uint16_t y, uint16_t w,uint16_t h, uint32_t col) {
	uint8_t* v = VMEM + VGA_STRIDE * y * VGA_BYTES + x * VGA_BYTES;
	uint8_t r = col >> 16;
	uint8_t g = col >> 8;
	uint8_t b = col & 0xff;
	for (uint16_t j = 0; j < h; j++) {
		
		uint8_t* vv = v;
		for (uint16_t i = 0; i < w; i++) {
			*vv++ = r;
			*vv++ = g;
			*vv++ = b;
		}
		
		v += VGA_STRIDE * VGA_BYTES;
	}
}
/*
void gfx_text(uint16_t x, uint16_t y, const char* str, uint8_t col, uint8_t fontNo) {
}
*/

void bochs_demo()
{
	uint8_t* p = VMEM;
	for (size_t y=0; y < VGA_HEIGHT; y++) {
		for (size_t x=0; x < VGA_WIDTH; x++) {
			*(p++) = x & 0xFF;
			*(p++) = y & 0xFF;
			*(p++) = 0xff;
		}
	}
	
	gfx_hline( 20, 20, 400, 0xFFFFFF );
	gfx_vline( 20, 20, 300, 0xFFFFFF );
	gfx_hline( 20, 319, 400, 0xFFFFFF );
	gfx_vline( 419, 20, 300, 0xFFFFFF );
	
	gfx_rect( 21, 21, 398, 20, 0xffbbbb );
	gfx_rect( 21, 41, 398, 278, 0xeeeeee );
	
	gfx_rect( 81, 81, 40, 40, 0x0000FF );
	gfx_rect( 131, 81, 40, 40, 0x00FF00 );
	gfx_rect( 181, 81, 40, 40, 0xFF0000 );
}

void BgaWriteRegister(uint16_t IndexValue, uint16_t DataValue)
{
    cpu_out16(VBE_DISPI_IOPORT_INDEX, IndexValue);
    cpu_out16(VBE_DISPI_IOPORT_DATA, DataValue);
}

uint16_t BgaReadRegister(uint16_t IndexValue)
{
    cpu_out16(VBE_DISPI_IOPORT_INDEX, IndexValue);
    return cpu_in16(VBE_DISPI_IOPORT_DATA);
}

int BgaIsAvailable(void)
{
    return (BgaReadRegister(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID5);
}

void BgaSetVideoMode(uint32_t Width, uint32_t Height, uint32_t BitDepth, bool UseLinearFrameBuffer, bool ClearVideoMemory)
{
    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    BgaWriteRegister(VBE_DISPI_INDEX_XRES, Width);
    BgaWriteRegister(VBE_DISPI_INDEX_YRES, Height);
    BgaWriteRegister(VBE_DISPI_INDEX_BPP, BitDepth);
    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED |
        (UseLinearFrameBuffer ? VBE_DISPI_LFB_ENABLED : 0) |
        (ClearVideoMemory ? 0 : VBE_DISPI_NOCLEARMEM));
}

void BgaSetBank(uint16_t BankNumber)
{
    BgaWriteRegister(VBE_DISPI_INDEX_BANK, BankNumber);
}

uint8_t* BgaGetFBAddr() {
	// The address of the framebuffer is not fixed, and must be read from the first PCI base address register (BAR 0 of device 0x1234:0x1111).
	return (uint8_t*)0xFD000000;
}


/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */