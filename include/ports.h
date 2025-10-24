#include <stdint.h>

void outb(uint16_t, uint8_t);
uint8_t inb(uint16_t);
void outw(uint16_t, uint16_t);
uint16_t inw(uint16_t);
uint32_t inl(uint16_t);
void outl(uint16_t, uint32_t);
