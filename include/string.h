#include <stddef.h>
#include <stdint.h>
#ifndef STRING_H
#define STRING_H

int strcmp(const char *, const char *);
size_t strlen(const char* );
int strcmpBySize(uint8_t* str1, uint8_t* str2, int len);

#endif
