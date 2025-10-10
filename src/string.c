#include "../include/string.h"
#include <stdint.h>
#include <stddef.h>

int strcmp(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }
    return (int)(*str1) - (int)(*str2);
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

int strcmpBySize(uint8_t* str1, uint8_t* str2, int len) {
  for (int i = 0; i < len; i++) {
    if (str1[i] != str2[i]) {
      return 0;
    }
  }
  return 1;
}

int strcmp2(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }
    return (int)(*str1) - (int)(*str2);
}