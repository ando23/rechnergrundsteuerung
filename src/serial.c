// Copyright 2022-2024; for authors see bottom
// Licence: MIT

#include "serial.h"

#include <stdarg.h>

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

void color3_on() {
	kputc( 0x1b );
	kputc( '[' );
	kputc( '3' );
	kputc( '1' );
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
	
	if (eax == 0) {
		kputc('0');
		return;
	}

	for (int i=3; i>=0; i--) {
		uint32_t v = (eax >> (8*i));
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
void kputl_dec(uint32_t eax) {
	char dec_buf[16] = {'0', 0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
	char* ptr = dec_buf;
	
	if (eax == 0) {
		kputc('0');
		return;
	}
	
	// Einzelne Stellen herausfischen:
	while (eax > 0) {
		uint8_t c = eax % 10;
		*ptr = c + '0';
		ptr++;
		eax = eax / 10;
	}

	// String umdreht ausgeben
	for (char* rc = ptr-1; rc >= dec_buf; rc--) {
		kputc(*rc);
	}
}

void kprintf(const char* format, ...) {
	va_list argp;
	va_start(argp, format);
	while (*format != '\0') {
		if (*format == '%') {
			format++;
			if (*format == '\0')
				break;
			if (*format == '%') {
				kputc('%');
			}
			else if (*format == 'c') {
				char char_to_print = va_arg(argp, int);
				kputc(char_to_print);
			}
			else if (*format == 'd') {
				uint32_t value = va_arg(argp, int);
				kputl_dec(value);
			}
			else if (*format == 'x' || *format == 'p') {
				uint32_t value = va_arg(argp, int);
				kputl(value);
			}
			else if (*format == 's') {
				uint32_t value = va_arg(argp, int);
				if (value != 0)
					kputs((char*)value);	// gefährlich
			}
			else {
				//NOT_IMPLEMENTED_EXCEPTION;
				break;
			}
		}
		else if (*format == '^') {
			format++;
			if (*format == '7') {
				color_off();
			}
			else if (*format == '1') {
				color_on();
			}
			else if (*format == '2') {
				color2_on();
			}
			else if (*format == '3') {
				color3_on();
			}
			else {
				kputc('^');
				kputc(*format);
			}
		}
		else if (*format == '\\') {
			format++;
			if (*format == '\0')
				break;
			switch (*format) {
				case '\\':	kputc('\\'); break;
				case 'r':	kputc('\r'); break;
				case 'n':	kputc('\n'); break;
				case 't':	kputc('\t'); break;
			}
		}
		else {
			kputc(*format);
		}
		format++;
	}
	va_end(argp);
}


/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */