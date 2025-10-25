#include <stdint.h>
#include "../include/paging.h"

extern void enablePaging();

extern uint32_t framebuffer;
extern uint32_t bbp;
extern uint32_t bbl;

uint32_t* pPD = (uint32_t*) 0x7000000;


int mapPhytoVirt(uint32_t phy, uint32_t virt, bool cache){
  uint32_t direntry = (virt >> 22) & 0x3ff;
  uint32_t pageentry = (virt >> 12) & 0x3ff;
  uint32_t* pPT = (uint32_t*) ((direntry + 1) * 4096 + (uint32_t)pPD);
  pPD[direntry] = (uint32_t)pPT & 0xfffff000 | 0xf;
  pPT[pageentry] = phy & 0xfffff000 | cache << 4 | 0xf;
  return 0;
}

int initPaging() {
  int iaddr;
  for (int i = 0x30000; i < 0x80000 ; i += 0x1000) {
    mapPhytoVirt(i, i, 0);
  }
  for (int i = 0; i < (bbl * 768) ; i += 0x1000){
    mapPhytoVirt(framebuffer + i, framebuffer + i, 0);
  }
  for (int i = 0x7000000; i < 0x7401000; i += 0x1000) {
    mapPhytoVirt( i, i, 0);
  }
  for (int i = 0; i < 0x10000; i += 0x1000) {
    mapPhytoVirt(i, i, 0);
  }
  enablePaging();
  return 0;
}
