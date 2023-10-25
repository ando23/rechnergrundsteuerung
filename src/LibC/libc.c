#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "errno.h"

/*
pid_t fork() {
	return ENOSYS;
}
*/

void memcpy(char* target, const char* source, size_t count) {
	for (size_t i = 0; i<count; i++) {
		target[i] = source[i];
	}
}

void memmove(char* target, const char* source, size_t count) {
	for (size_t i = 0; i<count; i++) {
		target[i] = source[i];
	}
}

void memset(char* target, char value, size_t count) {
	for (size_t i = 0; i<count; i++) {
		target[i] = value;
	}
} 

void memset16(uint16_t* target, uint16_t value, size_t count) {
	for (size_t i = 0; i<count; i++) {
		target[i] = value;
	}
}

//void strcat(char*, const char*);
//void strchr(const char *s, int c);
int strcmp(const char* p, const char* q) {
	while (*p && *q) {
		if (*p != *q)
			return *q - *p;
		p++;
		q++;
	}
	return 0;
}
//int strcmpi(const char*, const char*);
//void strcoll(const char*, const char*);
//char* strcpy(char*, const char*);
//size_t strcspn(const char*, const char*);
//char* strdup(const char*);
//char* strerror(int errnum);
//strftime(..)
//int stricmp(const char*, const char*);
size_t strlen(const char* s) {
	const char* p = s;
	for (; *p; p++) ;
	return (size_t)(p-s);
}
//strlwr(..)
//char* strncat(char*, const char*, size_t);
//int strncmp(const char*, const char*, size_t);
//strncmpi(..);
//char* strncpy(char*, const char*, size_t);
//strnicmp(..)
//strnset

//double atof(const char*);
//int atoi(const char*);
//long atol(const char*);


//int open(const char* path, int oflag);
//int read(int filedes, void *buf, unsigned int count);
//int write(int filedes, const void* buf, unsigned int count);
