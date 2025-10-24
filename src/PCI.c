#include <stdint.h>
#include "../include/ports.h"
#include "../include/terminal.h"
#include "../include/pci.h"

uint16_t pciReadConfigWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
  
    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
  
    // Write out the address
    outl(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

int pciCheckFunc(uint8_t bus, uint8_t slot){
  uint16_t word = pciReadConfigWord(bus, slot, 0, 0xa);
  word = (word >> 8) & 0xff;
  printNum(word);
  printChar(' ');
  word = pciReadConfigWord(bus, slot, 0, 0xa);
  word = word & 0xff;
  printNum(word);
  printChar(' ');
  word = pciReadConfigWord(bus, slot, 0, 0x8);
  word = (word >> 8) & 0xff;
  printNum(word);
  printChar('\n');
  return 0;
}
