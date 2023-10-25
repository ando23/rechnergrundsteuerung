// Copyright 2023 sysa. For authors see bottom.
// Licence: MIT

// constants
#define NULL nullptr

// types
typedef wchar_t
typedef wint_t
typedef char8_t
typedef char16_t
typedef char32_t
typedef mbstate_t


// functions: string manipulation
char* strcpy(char *dst, const char* src);
char* strncpy(char* dst, const char* src, size_t n);
char* strcat(char* s1, const char* s2);
char* strncat(char* s1, const char* s2, size_t n);
strxfrm

// functions: string examination
size_t strlen(const char* s);
int strcmp(const char* p, const char* q);
int strncmp(const char* p, const char* p, size_t n);
strcoll
char* strchr(const char* s, int c);
strrchr
strspn
strcspn
strpbrk
strstr
strtok

// functions: miscellaneous
char* strerror(int errnum);

// functions: memory manipulation
void memset(char* target, char value, size_t count);
void memset16(uint16_t* target, uint16_t value, size_t count);
void memcpy(char* target, const char* source, size_t count);
void memmove(char* target, const char* source, size_t count);
int memcmp(const char* p, const char* q, size_t count);
void* memchr(const char* s, int c, size_t n);

// functions: multibyte functions

// functions: numeric conversions
double atof(const char* s);
int atoi(const char* s);
long int atol(const char* s);
long int atoll(const char* s);
strtof
strtod
strtold
strtol
strtoll
strtoul
strtoull

// functions: popular extensions
bzero
memccpy
mempcpy
strcasecmp
strcat_s
strcpy_s
strdup
strndup
strerror_r
stricmp
strlcpy
strlcat
strsignal
strtok_r


/* Authors:
 *	Andreas Herzig
 */