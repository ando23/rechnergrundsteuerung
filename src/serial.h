// Copyright 2022 Andreas Herzig
// Licence: MIT

#pragma once

#include <stdint.h>

void init_kcom();


// Ein Zeichen ausgeben
void kputc(uint8_t al);

void color_on();
void color_off();
void color2_on();


// String ausgeben
void kputs(char* s);

// String ausgeben
void kputs2(char* s);

// Newline ausgeben
void kputnl();

// Wert als Hex ausgeben
void kputhexc(uint8_t al);

// Wert als Hex ausgeben
void kputl(uint32_t eax);

// Hexdump of memory
void kputhd(void* adresse, uint32_t anzahl);

// Wert als Hex ausgeben
void kputb(uint8_t value);

// Wert dezimal ausgeben
void kputb_dec(uint8_t al);

void kprintf(const char* pszFormat, ...);

