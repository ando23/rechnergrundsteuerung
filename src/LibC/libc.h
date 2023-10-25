void memcpy(char* target, const char* source, size_t count);
void memmove(char* target, const char* source, size_t count);
void memset(char* target, char value, size_t count);
void memset16(uint16_t* target, uint16_t value, size_t count);

size_t strlen(const char* s);
int strcmp(const char* p, const char* q);

// unistd.h
//pid_t fork();

#define ENOSYS -666
