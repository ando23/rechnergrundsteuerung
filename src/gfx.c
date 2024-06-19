// Copyright 2022-2023; for authors see bottom
// Licence: MIT

#include <stddef.h>
#include <stdint.h>
#include "cpu.h"
#include "gfx.h"
#include "gfx_vbe.h"
#include "serial.h"
#include "string.h"

#define VMEM ((uint8_t*)0xA0000)
#define VGA_WIDTH	320
#define VGA_HEIGHT	200


void gfx_init() {
	if (BgaIsAvailable()) {
		kputs("gfx: bochs!\n");
		BgaSetVideoMode( 640, 480, VBE_DISPI_BPP_24, true, true );
		
		char* p = BgaGetFBAddr();
		for (size_t i=0; i<640*480; i++) {
			*(p++) = 0xff;
			*(p++) = 0x7f;
			*(p++) = 0xff;
		}
	}
	else {
		kputs("gfx: no bochs\n");
		// Erstmal nur Mode13 (320x200x8)
		/*
		asm(
			"mov ax, 13			\r\n"
			"int 0x10			\r\n"
			);
		*/
	}
}

void gfx_clrscr(uint8_t col) {
	memset( VMEM, col, VGA_WIDTH * VGA_HEIGHT);
}

void gfx_put_pixel(uint16_t x, uint16_t y, uint8_t col) {
	VMEM[ y * VGA_WIDTH + x ] = col;
}

/*
void gfx_hline(uint16_t x, uint16_t y, uint16_t w, uint8_t col) {
}
void gfx_vline(uint16_t x, uint16_t y, uint16_t h, uint8_t col) {
}
void gfx_line(uint16_t x, uint16_t y, uint16_t nx, uint16_t ny, uint8_t col) {
}
void gfx_rect(uint16_t x, uint16_t y, uint16_t w,uint16_t h, uint8_t col) {
}

void gfx_text(uint16_t x, uint16_t y, const char* str, uint8_t col, uint8_t fontNo) {
}
*/


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

char* BgaGetFBAddr() {
	// The address of the framebuffer is not fixed, and must be read from the first PCI base address register (BAR 0 of device 0x1234:0x1111).
	return (char*)0xE0000000;
}


/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */