#include <stdint.h>
#include "../include/ports.h"
#include "../include/terminal.h"
#include "../include/pci.h"

struct pciDevice bus0[32];

uint32_t pciReadConfigDWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint32_t tmp = 0;
  
    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
  
    // Write out the address
    outl(0xCF8, address);
    // Read in the data
    tmp = inl(0xcfc);
    return tmp;
}

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
uint8_t pciReadConfigByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint8_t tmp = 0;
  
    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
  
    // Write out the address
    outl(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = (uint8_t)((inl(0xCFC) >> ((offset & 3) * 8)) & 0xFF);
    return tmp;
}
int enumPci() {
  for (int i = 0; i < 32; i++) {
    if ( 0xffff != pciReadConfigWord(0, i, 0, 0)) {
      bus0[i].vendorID = pciReadConfigWord(0, i, 0, 0);
      bus0[i].deviceID = pciReadConfigWord(0, i, 0, 0x2);

      bus0[i].command = pciReadConfigWord(0, i, 0, 0x4);
      bus0[i].status = pciReadConfigWord(0, i, 0, 0x6);

      bus0[i].revisionID = pciReadConfigByte(0, i, 0, 0x8);
      bus0[i].progIF = pciReadConfigByte(0, i, 0, 0x9);
      bus0[i].subclass = pciReadConfigByte(0, i, 0, 0xa);
      bus0[i].class = pciReadConfigByte(0, i, 0, 0xb);

      bus0[i].cacheLineSize = pciReadConfigByte(0, i, 0, 0xc);
      bus0[i].latencyTime = pciReadConfigByte(0, i, 0, 0xd);
      bus0[i].headerType = pciReadConfigByte(0, i, 0, 0xe);
      bus0[i].BIST = pciReadConfigByte(0, i, 0, 0xf);

      bus0[i].BAR0 = pciReadConfigDWord(0, i, 0, 0x10);
      bus0[i].BAR1 = pciReadConfigDWord(0, i, 0, 0x14);
      bus0[i].BAR2 = pciReadConfigDWord(0, i, 0, 0x18);
      bus0[i].BAR3 = pciReadConfigDWord(0, i, 0, 0x1c);
      bus0[i].BAR4 = pciReadConfigDWord(0, i, 0, 0x20);
      bus0[i].BAR5 = pciReadConfigDWord(0, i, 0, 0x24);

      bus0[i].cardBusCisPointer = pciReadConfigDWord(0, i, 0, 0x28);
      bus0[i].subsystemVendorID = pciReadConfigWord(0, i, 0, 0x2c);
      bus0[i].subsystemID = pciReadConfigWord(0, i, 0, 0x2e);
      bus0[i].expansionRomBaseAddress = pciReadConfigDWord(0, i, 0, 0x30);
      bus0[i].capabilitiesPonter = pciReadConfigByte(0, i, 0, 0x34);
      bus0[i].RES1 = 'R';
      bus0[i].RES2 = 'RR';
      bus0[i].RES3 = 'RRRR';
      bus0[i].interruptLine = pciReadConfigByte(0, i, 0, 0x3c);
      bus0[i].interruptPIN = pciReadConfigWord(0, i, 0, 0x3d);
      bus0[i].minGrant = pciReadConfigByte(0, i, 0, 0x3e);
      bus0[i].maxLatency = pciReadConfigByte(0, i, 0, 0x3f);
    } else {
      bus0[i].vendorID = 'N';
    }
  }
  return 0;
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
  printChar(' ');
  word = pciReadConfigWord(bus, slot, 0, 0xe) & 0xff;
  printHex(word);

  printChar('\n');
  return 0;
}
