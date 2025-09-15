#include <stdint.h>
#include "../include/acpi.h"
#include "../include/terminal.h"
#include "../include/string.h"


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

struct ACPISDTHeader* findFADT(struct ACPISDTHeader* header){
  uint32_t entries = ((header->Length) - sizeof(header));
  struct ACPISDTHeader* base = (struct ACPISDTHeader*)(header->rest);
  for (int i = 0; i < entries; i++) {
    if (strcmpBySize((base + i)->Signature, "FACP", 4)) {
      return (base + i);
    }
  }
  return NULL;
}

int initAcpi() {
  struct RSDP_t* rsdp = findRSDP();
//  for (int i = 0; i < 5; i++) {
//    printChar((rsdp->Signature)[i]);
//  }
  struct ACPISDTHeader* rsdt = (struct ACPISDTHeader*)(rsdp->RsdtAddress);
  for (int i = 0; i < 4; i++) {
    printChar((rsdt->Signature)[i]);
  }
  struct ACPISDTHeader* fadt = findFADT(rsdt);
  if (fadt == NULL) {
    printString("Fuck");
  }
  for (int i = 0; i < 4; i++) {
    printChar((fadt->Signature)[i]);
  }
  return 0;
}


