#include <stdint.h>
#include "../include/vga.h"
#include "../include/string.h"
#include "../include/ports.h"
#include "../include/idt.h"
#include "../include/isr.h"
#include "../include/acpi.h"
#include "../include/terminal.h"
#include "../include/keyboard.h"

void main() {
/*  for (int i = 0; i < 0x1000 * 576; i++) {
    switch (i % 3) {
      case 0:
        *(uint8_t*)(i + 0x64000) = 0x00;
        break;
      case 1:
        *(uint8_t*)(i + 0x64000) = 0x00;
        break;
      case 2:
        *(uint8_t*)(i + 0x64000) = 0xff;
        break;
    }
  }
*/
  isr_install();
  asm volatile("sti");

  for (int y = 0; y < 768; y++) {
    for (int x = 0; x < 1024; x++) {
      printPixel(x, y, 0x0000ff);
    }
  }
//  putChar(0, 0, 0xff00ff, 0x0000ff, 'a');
//  printString("SHELL ");
  initAcpi();
  init_keyboard();
//  asm volatile("int $31");
//  int x = 1/0;

//  asm volatile("cli");

  return;

}
