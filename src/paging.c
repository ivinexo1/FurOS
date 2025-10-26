#include <stdint.h>
#include "../include/paging.h"

extern void enablePaging();

extern uint32_t kernel_loc;
extern uint32_t kernel_size;
extern uint32_t framebuffer;
extern uint32_t bbp;
extern uint32_t bbl;

uint32_t* pPD = (uint32_t*) 0x100000;
uint8_t* pPB = (uint8_t*) 0x10000;


int mapPhytoVirt(uint32_t phy, uint32_t virt, bool cache){
  uint32_t direntry = (virt >> 22) & 0x3ff;
  uint32_t pageentry = (virt >> 12) & 0x3ff;
  uint32_t* pPT = (uint32_t*) ((direntry + 1) * 4096 + (uint32_t)pPD);
  pPD[direntry] = (uint32_t)pPT & 0xfffff000 | 0xf;
  pPT[pageentry] = phy & 0xfffff000 | cache << 4 | 0xf;
  if (phy <= 0x8000000) {
    pPB[phy >> 15] |= 1 << ((phy >> 12) & 0b111);
  }
  return 0;
}

int demapPage(uint32_t virt) {
  uint32_t* pPT = (uint32_t*)(pPD[virt >> 22] & 0xfffff000);
  uint32_t phy = pPT[(virt >> 12) & 0x3ff] & 0xfffff000;
  pPT[(virt >> 12) & 0x3ff] = 0;
  if (phy <= 0x8000000) {
    pPB[phy >> 15] &= 0xff ^ (1 << ((phy >> 12) & 0b111));
  }
  return 0;
}

int initPaging() {
  int iaddr;
  for (int i = kernel_loc; i < kernel_loc + 0x1000*kernel_size; i += 0x1000) {
    mapPhytoVirt(i, i, 0);
  }
  for (int i = 0; i < (bbl * 768) ; i += 0x1000){
    mapPhytoVirt(framebuffer + i, framebuffer + i, 0);
  }
  for (int i = 0x100000; i < 0x100000 + 0x401000; i += 0x1000) {
    mapPhytoVirt(i, i, 0);
  }
  for (int i = 0; i < 0x20000; i += 0x1000) {
    mapPhytoVirt(i, i, 0);
  }
  enablePaging();
  return 0;
}
