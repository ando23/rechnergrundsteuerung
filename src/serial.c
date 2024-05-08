// Copyright 2022-2024; for authors see bottom
// Licence: MIT

#include "serial.h"

#include "cpu.h"
#include "string.h"

#define COM1	0x3F8
#define COM2	0x2F8
#define COM3	0x3E8
#define COM4	0x2E8

#define PORT	0x3F8

#define uint8_t	unsigned char
#define uint16_t	unsigned short
#define uint32_t	unsigned int


void init_kcom() {
	cpu_out8(PORT + 1, 0x00);	// Disable all interrupts
	cpu_out8(PORT + 3, 0x80);	// Enable DLAB (set baud rate divisor)
	cpu_out8(PORT + 0, 0x03);	// Set divisor to 3 (lo byte) 38400 baud
	cpu_out8(PORT + 1, 0x00);	//                  (hi byte)
	cpu_out8(PORT + 3, 0x03);	// 8 bits, no parity, one stop bit
	cpu_out8(PORT + 2, 0xC7);	// Enable FIFO, clear them, with 14-byte threshold
	cpu_out8(PORT + 4, 0x0B);	// IRQs enabled, RTS/DSR set
	cpu_out8(PORT + 4, 0x1E);	// Set in loopback mode, test the serial chip
	cpu_out8(PORT + 0, 0xAE);	// Test serial chip (send byte 0xAE and check if serial returns same byte)

	// Check if serial is faulty (i.e: not same byte as sent)
	uint8_t inp = cpu_in8( PORT );
	if (inp != 0xAE) {
		//FIXME Log error
		return;
	}
	
	// If serial is not faulty set it in normal operation mode
	// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
	cpu_out8(PORT + 4, 0x0F);
}

// Ein Zeichen ausgeben
// input:	al
void kputc(uint8_t al) {
	uint8_t state;
	while (1) {
		// Prüfe ob bereit
		state = cpu_in8( PORT+5 );
		state &= 0x20;
		if (state != 0) break;
	}
	cpu_out8( PORT, al );
}

void color_on() {
	kputc( 0x1b );
	kputc( '[' );
	kputc( '3' );
	kputc( '6' );
	kputc( 'm' );
}
	
void color_off() {
	kputc( 0x1b );
	kputc( '[' );
	kputc( '0' );
	kputc( 'm' );
}

void color2_on() {
	kputc( 0x1b );
	kputc( '[' );
	kputc( '3' );
	kputc( '2' );
	kputc( 'm' );
}


// String ausgeben
// input:	s	psz
void kputs(char* s) {
	color_on();
	kputs2(s);
	color_off();
}

// String ausgeben
// input:	s
void kputs2(char* s) {
	while (*s) {
		kputc(*s);
		s++;
	}
}

// Newline ausgeben
// input:	none
void kputnl() {
	kputc( '\r' );
	kputc( '\n' );
}


// Wert als Hex ausgeben
void kputhexc(uint8_t al) {
	uint8_t hi = al >> 4;
	if (hi < 10)
		kputc( hi + '0');
	else
		kputc( hi - 10 + 'A' );
	uint8_t lo = al & 0xf;
	if (lo < 10)
		kputc( lo + '0');
	else
		kputc( lo - 10 + 'A' );
}

// Wert als Hex ausgeben
// input:	eax	32-Bit-Wert
void kputl(uint32_t eax) {
	kputc('0');
	kputc('x');

	for (int i=4; i>=0; i--) {
		uint32_t v = eax >> (4*i);
		kputhexc( (uint8_t)(v & 0xff) );
	}
}

// Hexdump of memory
void kputhd(void* adresse, uint32_t anzahl) {
	//TODO schön machen
	uint8_t* ptr = (uint8_t*)adresse;
	for (uint32_t i=0; i<anzahl; i++) {
		if ((i&16) == 0) {
			kputl(*((uint32_t*)ptr));
			kputc(0x20);
		}
		kputhexc( *(ptr + i) );
		
		if ((i&16) == 0) {
			kputnl();
		}
		else {
			kputc(0x20);
		}
		ptr++;
	}
	kputnl();
}


// Wert als Hex ausgeben
// input:	value	8-Bit-Wert
void kputb(uint8_t value) {
	kputc('0');
	kputc('x');
	kputhexc(value);
}

// Wert dezimal ausgeben
// input:	al	8-Bit-Wert
void kputb_dec(uint8_t al) {
	char kputb_dec_buf[4] = {'0', 0,0,0};
	char* ptr = kputb_dec_buf;
	
	// Einzelne Stellen herausfischen:
	while (al > 0) {
		uint8_t c = al % 10;
		*ptr = c + '0';
		ptr++;
		al = al / 10;
	}

	// String umdreht ausgeben
	//char outb [5];
	//char* pout = outb;
	for (char* rc = ptr-1; rc >= kputb_dec_buf; rc--) {
		//*pout = *rc;
		//pout++;
		kputhexc(*rc);
	}
	//*pout = 0;
	//kputs( kputb_dec_buf );
}

void kprintf(const char* pszFormat, ...) {
	char pszBuffer[1024];
	memset(pszBuffer, 0, sizeof(pszBuffer));
	
	// snprintf( pszBuffer, sizeof(pszBuffer), pszFormat, va... );
	
	kputs(pszBuffer);
}


/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */