#include <stdint.h>
#include "../include/mouseDriver.h"
#include "../include/ports.h"
#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/terminal.h"

uint8_t mouse_buttons = 0;
static uint8_t mouse_packet[3];
static int mouse_packet_idx = 0;
int mouseX = 0;
int mouseY = 0;

// https://wiki.osdev.org/Mouse_Input

// data in port 0x60
// 0x64 bit n0 == 1 = data available
// n1 == 0x20 = from mouse

// formatting of bytes
// 1 = Y overflow, X overflow, Y sign bit, X sign bit, Always 1, Middle Btn, Right Btn, Left Btn
// 2 = X movement
// 3 = Y movement
// 4 = optional

// p == packet of 3 bytes
static void parse_mouse_packet(uint8_t p[3]) {
    if (p[0] & 0x10) {
        // 11000000 = XY overflow
        return;
    }

    // delta xy
    int8_t dx = (int8_t)p[1];
    int8_t dy = (int8_t)p[2];

    mouseX += dx;
    mouseY -= dy;

    // clamp to screen bounds
    if (mouseX < 0) mouseX = 0;
    if (mouseY < 0) mouseY = 0;
    if (mouseX > 1024) mouseX = 1024;
    if (mouseY > 768) mouseY = 768;

    mouse_buttons = p[0] & 0x07; // middle, right, left
  printNum(dx);
}

void mouse_callback(registers_t *regs) {
    uint8_t status = inb(0x64);
    if (!(status & 0x01)) return;   // no data available
    if (!(status & 0x20)) return;   // not from mouse (AUX)

    uint8_t b = inb(0x60);
    mouse_packet[mouse_packet_idx++] = b;
    if (mouse_packet_idx >= 3) {
        mouse_packet_idx = 0;
        parse_mouse_packet(mouse_packet);
    }
}

void init_mouse() {
  while(inb(0x64) & 2);
  outb(0x64, 0x20);
//  while((inb(0x64) & 1) != 1);
  uint8_t cmq = inb(0x60);

  printHex(cmq);
  printChar(' ');
  cmq = cmq & (0xff ^ 0x20);
  cmq = cmq | 2;
  printHex(cmq);
  printChar(' ');

  while(inb(0x64) & 2);
  outb(0x64, 0x60);
  while(inb(0x64) & 2);
  outb(0x60, cmq);

  while(inb(0x64) & 2);
  outb(0x64, 0x20);
//  while((inb(0x64) & 1) != 1);
  printHex(inb(0x60));
  printChar('\n');

  while(inb(0x64) & 2);
  outb(0x64, 0xa8);

/*  outb(0x64, 0x20);
  uint8_t cmq = inb(0x64);
  cmq = cmq & 0b11101111;
  cmq = cmq | 0b00000001;
  printHex(cmq);
  printChar(' ');*/
/*  
  outb(0x64, 0x60);
  while(inb(0x64) & 2);
  outb(0x60, cmq);

  outb(0x64, 0x20);
  printHex(inb(0x64));

  printChar('\n');

  outb(0x64, 0xd4);
  outb(0x60, 0xf2);
  while (inb(0x60) != 0xfa) {
  }
  printHex(inb(0x60));
  outb(0x64, 0xd4);
  outb(0x60, 0xf3);
  while (inb(0x60) != 0xfa) {
  }
  outb(0x64, 0xd4);
  outb(0x60, 200);
  while (inb(0x60) != 0xfa) {
  }
  printString("Set sample rate to 200\n");
  outb(0x64, 0xd4);
  outb(0x60, 0xf3);
  while (inb(0x60) != 0xfa) {
  }
  outb(0x64, 0xd4);
  outb(0x60, 100);
  while (inb(0x60) != 0xfa) {
  }
  printString("Set sample rate to 100\n");
  outb(0x64, 0xd4);
  outb(0x60, 0xf3);
  while (inb(0x60) != 0xfa) {
  }
  outb(0x64, 0xd4);
  outb(0x60, 80);
  while (inb(0x60) != 0xfa) {
  }
  printString("Set sample rate to 80\n");
  outb(0x64, 0xd4);
  outb(0x60, 0xf2);
  while (inb(0x60) != 0xfa) {
  }
  printHex(inb(0x60));
*/
  while(inb(0x64) & 2);
  outb(0x64, 0xd4);
  while(inb(0x64) & 2);
  outb(0x60, 0xf4);
  while((inb(0x64) & 1) != 1);
  while (inb(0x60) != 0xfa);
  printString("Enabled packets\n");


 /* while(true) {
    printHex(inb(0x60));
    printChar('\n');
  }*/
  register_interrupt_handler(IRQ12, mouse_callback);
}
