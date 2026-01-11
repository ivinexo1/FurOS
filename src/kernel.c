#include <stdint.h>
#include "../include/vga.h"
#include "../include/string.h"
#include "../include/ports.h"
#include "../include/idt.h"
#include "../include/isr.h"
#include "../include/acpi.h"
#include "../include/terminal.h"
#include "../include/keyboard.h"
#include "../include/pci.h"
#include "../include/paging.h"
#include "../include/mouseDriver.h"

extern uint32_t kernel_loc;
extern uint32_t kernel_size;
extern uint32_t framebuffer;
extern uint32_t bbl;

void main() {
  for (int y = 0; y < 768; y++) {
    for (int x = 0; x < 1024; x++) {
      printPixel(x, y, 0x282b2e); // light gray
    }
  }

  printString("1. Setting up interrupts\n");
  isr_install();
  asm volatile("sti");

  printString("2. Starting ACPI\n");
  initAcpi();

  printString("3. Turning on Paging\n");
  initPaging();

  printString("4. Enumerating PCI\n");
  enumPci();

  printString("5. Initalising keyboard\n");
  init_keyboard();

  printString("6. Initalising mouse\n");
  init_mouse();

//  printNum(bus0[4].BAR5);

  printString("\nFurOS\nSHELL ");
 
//  for(int i = 0; i < 5; i++) {
//    for(int j = 0; j < 5; j++) {
//      printSprite(i*200, j*200);
//    }
//  }
//  asm volatile("int $31");
//  int x = 1/0;

//  asm volatile("cli");
  mapPhytoVirt(0x50000, 0x60000, 0);
  *((uint8_t*)0x60000) = 0xab;
  printHex(*((uint8_t*)0x60000));
  demapPage(0x60000);

  *((uint8_t*)0x50000) = 0xab;

  return;

}
