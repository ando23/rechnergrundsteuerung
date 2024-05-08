// Copyright 2022-2023; for authors see bottom
// Licence: MIT

#include "textmode.h"

#include "cpu.h"
#include "string.h"

#define KTEXTMEM	((char*)0xB8000)
#define KTEXTMAXX	80
#define KTEXTMAXY	25

uint16_t* VMEM = (uint16_t*)(0xB8000);
uint16_t cursor_x = 0;
uint16_t cursor_y = 0;
uint8_t cursor_col = 0x17;


void clrscr() {
	uint16_t v = (cursor_col << 8) + ' ';
	memset16(KTEXTMEM, v, KTEXTMAXX * KTEXTMAXY);
}

void disable_cursor() {
	cpu_out8( 0x3D4, 0xA );		// low cursor shape register
	cpu_out8( 0x3D5, 0x20 );	// bits 6-7 unused, bit 5 disables the cursor, bits 0-4 control the cursor shape
}

void set_color(uint8_t col) {
	cursor_col = col;
}

void set_cursor_position(uint16_t x, uint16_t y)
{
	if (x > KTEXTMAXX-1) x = KTEXTMAXX-1;
	if (y > KTEXTMAXY-1) y = KTEXTMAXY-1;

	cursor_x = x;
	cursor_y = y;
}


// Output a single character
void putc(uint8_t c) {
	if (cursor_x >= KTEXTMAXX) {
		cursor_x = 0;
		cursor_y++;
	}
	if (cursor_y >= KTEXTMAXY) {
		scrollup();
		cursor_y = (KTEXTMAXY-1);
	}
	
	if (c == '\n') { // NL auswerten
		cursor_x = 0;
		cursor_y++;
	}
	else if (c == '\r') { // CR auswerten
		cursor_x = 0;
	}
	else if (c >= 32) {
		uint16_t value = (cursor_col << 8) + c;
		VMEM[ cursor_y * KTEXTMAXX + cursor_x ] = value;
		cursor_x++;
	}
}


// Output a string
void puts(const char* psz) {
	for (; *psz; psz++) {
		putc(*psz);
	}
	putc('\n');
}


// Output a hex char without prefix
void puthexc(uint8_t c) {
	uint8_t hi = c >> 4;
	if (hi > 10)
		hi -= 10;
	putc( hi + 'A');
	uint8_t lo = c & 0xF;
	putc( lo + 'A');
}


// Output value of EAX as hex with prefix '0x'
void putl(uint32_t value) {
	putc('0');
	putc('x');

	for (int i=4; i>=0; i--) {
		uint32_t v = value >> (4*i);
		puthexc( (uint8_t)(v & 0xff) );
	}
}


// Output eax as decimal (uses stack as temp buffer)
void putld(uint32_t value) {
	// min 0
	// max 4.294.967.295	(10 Stellen)
	
	// Sonderfall 0
	if (value == 0) {
		putc('0');
		return;
	}
	
	char buffer[16];
	snprintf( buffer, sizeof(buffer), "%d", value );
	puts(buffer);
}


// Scroll the entire screen up one line and fill the bottom one with blanks
void scrollup() {
	//memmove( KTEXTMEM, KTEXTMEM + 2*KTEXTMAXX, 2*KTEXTMAXX*(KTEXTMAXY-1) );
	memmove( KTEXTMEM + 2*KTEXTMAXX, KTEXTMEM + 4*KTEXTMAXX, 2*KTEXTMAXX*(KTEXTMAXY-2) );
	uint16_t v = (cursor_col << 8) + ' ';
	memset16( KTEXTMEM + 2*KTEXTMAXX*(KTEXTMAXY-1), v, 2*KTEXTMAXX);
}


void printf(const char* pszFormat, ...) {
	char pszBuffer[1024];
	memset(pszBuffer, 0, sizeof(pszBuffer));
	
	// snprintf( pszBuffer, sizeof(pszBuffer), pszFormat, va... );
	
	puts(pszBuffer);
}


/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */