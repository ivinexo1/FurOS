#include <stdint.h>
#include "../include/vga.h"
#include "../include/string.h"
#include "../include/ports.h"
#include "../include/idt.h"
#include "../include/isr.h"
#include "../include/terminal.h"
#include "../include/keyboard.h"

void main() {
  isr_install();
  asm volatile("sti");
  for (int y = 0; y < 768; y++) {
    for (int x = 0; x < 1024; x++) {
      printPixel(x, y, 0x0000ff);
    }
  }
//  putChar(0, 0, 0xff00ff, 0x0000ff, 'a');
  printChar('S');
  printChar('H');
  printChar('E');
  printChar('L');
  printChar('L');
  init_keyboard();
//  asm volatile("int $31");
//  int x = 1/0;

//  asm volatile("cli");

  return;

}
