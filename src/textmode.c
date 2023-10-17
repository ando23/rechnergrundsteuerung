// Copyright 2022-2023; for authors see bottom
// Licence: MIT

#include "textmode.h"

#include "cpu.h"
#include "memory.h"

#define KTEXTMEM	0xB8000
#define KTEXTMAXX	80
#define KTEXTMAXY	25

uint16_t* VMEM = (uint16_t*)(0xB8000);
uint16_t cursor_x = 0;
uint16_t cursor_y = 0;
uint8_t cursor_col = 0x17;


void clrscr() {
	uint16_t v = (cursor_col << 8) + 0x20;
	memset16(KTEXTMEM, v, 80*25);
}

void disable_cursor() {
	cpu_out8( 0x3D4, 0xA );		// low cursor shape register
	cpu_out8( 0x3D5, 0x20 );	// bits 6-7 unused, bit 5 disables the cursor, bits 0-4 control the cursor shape
}

void set_color(uint8_t col) {
	cursor_col = col;
}

// Output a single character
void putc(uint8_t c) {
	if (c == 10) {	// NL auswerten
		cursor_x = 0;
		cursor_y++;
	}
	else if (c == 13) {	// CR auswerten
		cursor_x = 0;
	}
	else if (c >= 32) {
		uint16_t value = (cursor_col << 8) + c;
		VMEM[ cursor_y * KTEXTMAXX + cursor_x ] = value;
		cursor_x++;
	}
	
	if (cursor_x >= KTEXTMAXX) {
		cursor_x = 0;
		cursor_y++;
	}
	if (cursor_y >= KTEXTMAXY) {
		scrollup();
		cursor_y = (KTEXTMAXY-1);
	}
}


// Output a string
void puts(const char* psz) {
	for (; *psz; psz++) {
		putc(*psz);
	}
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
		kputhexc( (uint8_t)(v & 0xff) );
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
	
	//char buffer[16];
	//snprintf( buffer, sizeof(buffer), "%d", value );
	//puts(buffer);
}


// Scroll the entire screen up one line and fill the bottom one with blanks
void scrollup() {
	memmove( KTEXTMEM + 2*80, KTEXTMEM, 2*80*24 );
	uint16_t v = (cursor_col << 8) + 0x20;
	memset16( KTEXTMEM + 2*80*24, v, 2*80);
}


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
void printf(const char* pszFormat, ...) {
/*	
	push edi
	push ebp
	push ebx
	mov ebx, esp
	add ebx, 12
	mov ebp, ebx
	
	mov edi, eax
.loop:
	mov al, [edi]
	cmp al, 0
	je .end
	
	cmp al, '%'
	je .makro

.disp:
	call putc
.next:
	inc edi
	jmp .loop

.makro:
	inc edi
	mov al, [edi]
	cmp al, 0
	je .end
	cmp al, '%'
	je .disp
	cmp al, 'd'
	je .disp_u32
	cmp al, 'b'
	je .disp_u8
	cmp al, 'p'
	je .disp_p
	jmp .next

.disp_u8:
	push eax
	add ebp, 1
	mov eax, 0
	mov al, byte [ebp]
	call putld
	pop eax
	jmp .next

.disp_u32:
	push eax
	add ebp, 4
	mov eax, [ebp]
	call putld
	pop eax
	jmp .next
	
.disp_p:
	push eax
	add ebp, 4
	mov eax, [ebp]
	call putl
	pop eax
	jmp .next

.disp_s:
	push eax
	add ebp, 4
	mov eax, [ebp]
	call puts
	pop eax
	jmp .next
	
.end:
	pop ebx
	pop ebp
	pop edi
*/	
}





/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */