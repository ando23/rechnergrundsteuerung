// Copyright 2022-2023; for authors see bottom
// Licence: MIT

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Erstmal nur Mode13 (320x200x8)
void gfx_init();

void gfx_clrscr(uint8_t col);

void gfx_put_pixel(uint16_t x, uint16_t y, uint8_t col);

void gfx_hline(uint16_t x, uint16_t y, uint16_t w, uint8_t col);
void gfx_vline(uint16_t x, uint16_t y, uint16_t h, uint8_t col);
void gfx_line(uint16_t x, uint16_t y, uint16_t nx, uint16_t ny, uint8_t col);
void gfx_rect(uint16_t x, uint16_t y, uint16_t w,uint16_t h, uint8_t col);

void gfx_text(uint16_t x, uint16_t y, const char* str, uint8_t col, uint8_t fontNo);


// === BOCHS VGA ===
void BgaWriteRegister(uint16_t IndexValue, uint16_t DataValue);
uint16_t BgaReadRegister(uint16_t IndexValue);
int BgaIsAvailable(void);
void BgaSetVideoMode(uint32_t Width, uint32_t Height, uint32_t BitDepth, bool UseLinearFrameBuffer, bool ClearVideoMemory);
void BgaSetBank(uint16_t BankNumber);
