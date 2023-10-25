// Copyright 2023 sysa. For authors see bottom.
// Licence: MIT

// file access
FILE* fopen(const char* file, const char* mode);
FILE* freopen(const char* file, const char* mode, FILE* stream);
int fflush(FILE* stream);
int fclose(FILE* stream);
setbuf
setvbuf
fwide

// direct input/output
size_t fread(void *ptr, size_t size, size_t nmemb, FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);

// unformatted input/output
int fgetc(FILE* stream);
int getc(FILE* stream);
char* fgets(char *s, int n, FILE* stream);
int fputc(int c, FILE* stream);
int putc(int c, FILE* stream);
int fputs(const char* s, FILE* stream);
int getchar(void);
int putchar(int c);
int puts(const char* s);
int ungetc(int c, FILE* stream);

// formatted input/output
scanf
fscanf
sscanf
vscanf
vfscanf
vsscanf
int printf(const char* format, ...);
fprintf
sprintf
snprintf
int vprintf(const char* format, va_list arg);
vfprintf
vsprintf
vsnprintf
void perror(const char* s);

// file positioning
long int ftell(FILE* stream);
ftello
int fseek(FILE* stream, long int offset, int whence);
fseeko
fgetpos
fsetpos
void rewind(FILE* stream);

// error handling
void clearerr(FILE* stream);
int feof(FILE* stream);
int ferror(FILE* stream);

// operations on files
int remove(const char* filename);
int rename(const char* old_name, const char* new_name);
FILE* tmpfile(void);
char* tmpnam(char* s);


// constants
const EOF
const BUFSIZ
const FILENAME_MAX
const FOPEN_MAX
const _IOFBF
const _IOLBF
const _IONBF
const L_tmpnam
#define NULL nullptr
const SEEK_CUR
const SEEK_END
const SEEK_SET
const TMP_MAX


// variables
extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;


// member types
typedef void* FILE;
typedef int fpos_t;
//typedef size_t;



/* Authors:
 *	Andreas Herzig
 */