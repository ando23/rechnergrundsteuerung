// Copyright 2022-2024; for authors see bottom
// Licence: MIT

#include "string.h"
#include "memory.h"
#include <stdarg.h>


void memset(void* s, uint8_t c, size_t n) {
	uint8_t* p = (uint8_t*)s;
	for (; n; n--)
		*(p++) = c;
}
void memset16(void* s, uint16_t c, size_t n) {
	uint16_t* p = (uint16_t*)s;
	for (; n; n--)
		*(p++) = c;
}
void memcpy(void* dst, void* src, size_t n) {
	if (dst == src) return;
	if (n == 0) return;

	char* pDst = (char*)dst;
	char* pSrc = (char*)src;
	for (; n; n--)
		*(pDst++) = *(pSrc++);
}
void memmove(void* dst, void* src, size_t n) {
	if (dst == src) return;
	if (n == 0) return;

	if (dst < src) {
		memcpy(dst, src, n);
	}
	else {
		char* pDst = (char*)dst;
		char* pSrc = (char*)src;
		pDst += n;
		pSrc += n;
		for (; n; n--)
			*(--pDst) = *(--pSrc);
	}
}


char* pad_string(char* outBuffer, size_t padding, char padding_char) {
	if (padding == 0) return outBuffer;
	for (size_t p = 0; p < padding; p++) {
		*outBuffer = padding_char;
		*outBuffer++;
	}
	return outBuffer;
}


char* s_itoa(char* outBuffer, uint32_t value, int base, size_t padding, char padding_char) {
	pad_string(outBuffer, padding, padding_char);
	
	char buf[40];
	char *pBuf = buf;
	
	if (value == 0) {
		*(outBuffer++) = '0';
		return outBuffer;
	}

	//kputs("--\r\n");
	while (value > 0) {
		uint32_t x = value % base;
		
		if (x < 10)
			*pBuf = x + '0';
		else
			*pBuf = (x-10) + 'A';
		
		value /= base;
		pBuf++;
	}
	
	for (;pBuf > buf;)
		*(outBuffer++) = *(--pBuf);
	
	return outBuffer;
}


size_t snprintf(char* outBuffer, size_t buffer_size, const char* format, ...) {
	memset(outBuffer, 0, buffer_size);
	
	char* buf = outBuffer;
	char* end = outBuffer + buffer_size - 1;
	
	va_list argp;
	va_start(argp, format);
	while (*format != '\0' && buf != end) {
		if (*format == '%') {
			format++;
			if (*format == '\0')
				break;
			if (*format == '%') {
				*buf = '%';
			}
			else if (*format == 'c') {
				char char_to_print = va_arg(argp, int);
				*buf = char_to_print;
			}
			else if (*format == 'd') {
				uint32_t value = va_arg(argp, int);
				buf = s_itoa(buf, value, 10, 0, '0') - 1;
			}
			else if (*format == 'x') {
				uint32_t value = va_arg(argp, int);
				buf = s_itoa(buf, value, 16, 0, '0') - 1;
			}
			else {
				//NOT_IMPLEMENTED_EXCEPTION;
				break;
			}
		}
		else if (*format == '\\') {
			format++;
			if (*format == '\0')
				break;
			switch (*format) {
				case '\\':	*buf = '\\'; break;
				case 'r':	*buf = '\r'; break;
				case 'n':	*buf = '\n'; break;
				case 't':	*buf = '\t'; break;
			}
		}
		else {
			*buf = *format;
		}
		buf++;
		format++;
	}
	va_end(argp);
	
	return outBuffer - buf;
}


/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */