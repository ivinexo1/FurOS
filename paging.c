#include <stdint.h>

uint32_t* pPD = 0x10000;


int setPageDirectoryEntry(uint32_t* addressvirt, uint32_t addressphy, bool cached, bool ) {
  uint32_t lcached = (uint32_t) cached;
  uint32_t entry = (addressphy & 0xfffff000) | (lcached << 4) | 0x00F;
  *(pPD + addressvirt) = entry;
}


