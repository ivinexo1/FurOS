#include <stdint.h>
#include "../include/acpi.h"
#include "../include/terminal.h"
#include "../include/string.h"
#include "../include/ports.h"



uint16_t SLP_TYPa;
uint16_t SLP_TYPb;
uint16_t SLP_EN = 1<<13;

uint16_t S5a;
uint16_t S5b;
uint32_t PM1a_CNT;
uint32_t PM1b_CNT;


int RsdpChecksum(struct RSDP_t* rsdp) {
  uint8_t* ptr = (uint8_t*)rsdp;
  uint8_t checksum = 0;
  for (int i = 0; i < sizeof(struct RSDP_t); i++) {
    checksum += *ptr;
    ptr++;
  }
  if (checksum == 0) {
    return 0;
  }
  return 1;
}

struct RSDP_t* findRSDP() {
  uint32_t RsdpP = (uint32_t)*(uint8_t*)(0x40e);
  uint8_t RsdpID[8] = "RSD PTR ";
  for (uint32_t i = 0; i < 1024; i++) {
    if(strcmpBySize((uint8_t*)(RsdpP + i), RsdpID, 8)){
      if (RsdpChecksum((struct RSDP_t*)(RsdpP + i)) == 0) {
        return (struct RSDP_t*)(RsdpP + i);
      }
    }
  }
  for (uint32_t i = 0xe0000; i < 0xfffff; i++) {
    if(strcmpBySize((uint8_t*)i, RsdpID, 8)){
      if (RsdpChecksum((struct RSDP_t*)i) == 0) {
        return (struct RSDP_t*)i;
      }
    }
  }
  return NULL;
}

int SDTChecksum(struct ACPISDTHeader* ptr) {
  uint8_t checksum = 0;
  for (int i = 0; i < ptr->Length; i++) {
    checksum += ((char*)ptr)[i];
  }
  if (checksum == 0) {
    return 0;
  }
  return 1;
}

struct FADT* findFADT(struct RSDT* header){
  uint32_t entries = ((header->h.Length) - sizeof(header));
  struct FADT* base = (struct FADT*)(header->rest);
  for (int i = 0; i < entries; i++) {
    if (strcmpBySize((base + i)->h.Signature, "FACP", 4)) {
      if (SDTChecksum(&((base + i)->h)) == 0) {
        return (base + i);
      }
    }
  }
  return NULL;
}


uint8_t* findS5(struct DSDT* dsdt) {
  uint8_t* S5Addr = (uint8_t *) (dsdt + 36); // skip header
	int dsdtLength = (dsdt->h.Length) -36;
  for (int i = 0; i < dsdtLength; i++) {
    if (strcmpBySize(S5Addr, "_S5_", 4)) {
      return S5Addr;
    }
    S5Addr++;
  }
  return NULL;
}


int initAcpi() {
  struct RSDP_t* rsdp = findRSDP();
  if (rsdp != NULL) {
    printString("Found RSDP\n");
  } else {
    return -1;
  }

  struct RSDT* rsdt = (struct RSDT*)(rsdp->RsdtAddress);
  if (rsdt != NULL) {
    printString("Found ");
  } else {
    return -1;
  }
  for (int i = 0; i < 4; i++) {
    printChar((rsdt->h.Signature)[i]);
  }
  printChar('\n');

  struct FADT* fadt = findFADT(rsdt);
  if (fadt != NULL) {
    printString("Found ");
  }else {
    return -1;
  }
  for (int i = 0; i < 4; i++) {
    printChar((fadt->h.Signature)[i]);
  }
  printChar('\n');
  struct DSDT* dsdt = (struct DSDT*)fadt->Dsdt;
  if (dsdt != NULL) {
    printString("Found ");
  } else {
    return -1;
  }
  for (int i = 0; i < 4; i++) {
    printChar(((uint8_t*)(dsdt->h.Signature))[i]);
  }
  printChar('\n');
  uint8_t* S5 = findS5(dsdt);
  if (S5 != NULL) {
    printString("Found S5");
  } else {
    return -1;
  }
  printChar('\n');
//  outb(fadt->SMI_CommandPort, fadt->AcpiEnable);
  if (inw((unsigned int) fadt->PM1aControlBlock) == 0) {
    printString("ACPI enabled\n");
  } else {
    printString("ACPI disabled\n");
  }
  PM1a_CNT = fadt->PM1aControlBlock;
  PM1b_CNT = fadt->PM1bControlBlock;
  S5a = *(S5 + 5);
  S5b = *(S5 + 7);
//  outw(fadt->PM1aControlBlock, *(S5 + 5) | 0x2000);
//  outw(fadt->PM1bControlBlock, *(S5 + 7) | 0x2000);
  return 0;
}

int shutdown(){
  outw(PM1a_CNT, S5a | 0x2000); 
  outw(PM1b_CNT, S5b | 0x2000); 
  return 0;
}
