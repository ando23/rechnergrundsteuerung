// Copyright 2022-2023; for authors see bottom
// Licence: MIT

#include <stddef.h>
#include <stdint.h>
#include "cpu.h"
#include "gfx.h"

#include "memory.h"

#define VMEM ((uint8_t*)0xA0000)
#define VGA_WIDTH	320
#define VGA_HEIGHT	200

/*
// Erstmal nur Mode13 (320x200x8)
void gfx_init() {
	//asm( "mov ax, 0x13 \n int 0x10");
}

void gfx_clrscr(uint8_t col) {
	memset( VMEM, col, VGA_WIDTH * VGA_HEIGHT);
}

void gfx_put_pixel(uint16_t x, uint16_t y, uint8_t col) {
	VMEM[ y * VGA_WIDTH + x ] = col;
}

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


#define VBE_DISPI_BANK_ADDRESS          0xA0000
#define VBE_DISPI_BANK_SIZE_KB          64

#define VBE_DISPI_MAX_XRES              1024
#define VBE_DISPI_MAX_YRES              768

#define VBE_DISPI_IOPORT_INDEX          0x01CE
#define VBE_DISPI_IOPORT_DATA           0x01CF

#define VBE_DISPI_INDEX_ID              0x0
#define VBE_DISPI_INDEX_XRES            0x1
#define VBE_DISPI_INDEX_YRES            0x2
#define VBE_DISPI_INDEX_BPP             0x3
#define VBE_DISPI_INDEX_ENABLE          0x4
#define VBE_DISPI_INDEX_BANK            0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH      0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT     0x7
#define VBE_DISPI_INDEX_X_OFFSET        0x8
#define VBE_DISPI_INDEX_Y_OFFSET        0x9

#define VBE_DISPI_ID0                   0xB0C0
#define VBE_DISPI_ID1                   0xB0C1
#define VBE_DISPI_ID2                   0xB0C2
#define VBE_DISPI_ID3                   0xB0C3
#define VBE_DISPI_ID4                   0xB0C4

#define VBE_DISPI_DISABLED              0x00
#define VBE_DISPI_ENABLED               0x01
#define VBE_DISPI_VBE_ENABLED           0x40
#define VBE_DISPI_LFB_ENABLED           0x40
#define VBE_DISPI_NOCLEARMEM            0x80

#define VBE_DISPI_LFB_PHYSICAL_ADDRESS  0xE0000000


#define VBE_DISPI_BPP_4	(0x04)
#define VBE_DISPI_BPP_8	(0x08)
#define VBE_DISPI_BPP_15	(0x0F)
#define VBE_DISPI_BPP_16	(0x10)
#define VBE_DISPI_BPP_24	(0x18)
#define VBE_DISPI_BPP_32	(0x20) 


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
    return (BgaReadRegister(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID4);
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
