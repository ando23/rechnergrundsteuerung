// Copyright 2022-2023; for authors see bottom
// Licence: MIT

#include "memory.h"

void gte_set(struct GDT_Table_entry_t* entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	entry->base0 = (base & 0xFFFF);
	entry->base16 = (base >> 16) & 0xFF;
	entry->base24 = (base >> 24);
	
	entry->limit0 = (limit & 0xFFFF);
	//TODO check me
	entry->limit16_flags = ((limit >> 16) << 4) | (flags & 0xF);
	
	entry->access = access;
}


void memset(void *s, uint8_t c, size_t n) {
	uint8_t* p = (uint8_t*)s;
	for (; n; n--)
		*(p++) = c;
}
void memset16(void *s, uint16_t c, size_t n) {
	uint16_t* p = (uint16_t*)s;
	for (; n; n--)
		*(p++) = c;
}
void memcpy(void *dst, void *src, size_t n) {
	if (dst == src) return;
	if (n == 0) return;

	char * pDst = (char*)dst;
	char * pSrc = (char*)src;
	for (; n; n--)
		*(pDst++) = *(pSrc++);
}
void memmove(void *dst, void *src, size_t n) {
	if (dst == src) return;
	if (n == 0) return;
	
	if ( dst < src ) {
		memcpy(dst, src, n);
	}
	else {
		char * pDst = (char*)dst;
		char * pSrc = (char*)src;
		pDst += n;
		pSrc += n;
		for (; n; n--)
			*(--pDst) = *(--pSrc);
	}
}


/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */