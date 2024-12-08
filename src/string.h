#include <stddef.h>
#include <stdint.h>

void memset(void* s, uint8_t c, size_t n);
void memset16(void* s, uint16_t c, size_t n);
void memcpy(void* dst, void* src, size_t n);
void memmove(void* dst, void* src, size_t n);

// Display formatted text / non-standard makros!
//	input	eax	Formatstring
//			%c	char	dez
//			%d	u32	dez
//			%w	u16	dez
//			%b	u8	dez
//			%D	u32	hex
//			%W	u16	hex
//			%B	u8	hex
//			%p	u32	hex (pointer)
//			%s	string
size_t snprintf(char* outBuffer, size_t buffer_size, const char* pszFormat, ...);
