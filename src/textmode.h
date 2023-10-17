// Copyright 2022-2023; for authors see bottom
// Licence: MIT

#pragma once

#include <stdint.h>


// 
void clrscr();

void disable_cursor();

void set_color(uint8_t col);

// Output a single character
void putc(uint8_t c);

// Output a string
void puts(const char* psz);

// Output a hex char without prefix
void puthexc(uint8_t c);

// Output value of EAX as hex with prefix '0x'
void putl(uint32_t value);

// Output eax as decimal (uses stack as temp buffer)
void putld(uint32_t value);

// Scroll the entire screen up one line and fill the bottom one with blanks
void scrollup();

// Display formatted text / non-standard makros!
//	input	eax	Formatstring
//			%d	u32	dez
//			%w	u16	dez
//			%b	u8	dez
//			%D	u32	hex
//			%W	u16	hex
//			%B	u8	hex
//			%p	u32	hex (pointer)
//			%s	string
//	clobbers	eax
void printf(const char* pszFormat, ...);



/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */