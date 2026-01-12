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


//
//

void init_mouse() {
  //get I8042 configuration
  while(inb(0x64) & 2); //wait until ready to accept input
  outb(0x64, 0x20); //send command get configuration
  uint8_t cmq = inb(0x60); //get configuration byte

  cmq = cmq & (0xff ^ 0x20); //enable aux ps/2 port clock 
  cmq = cmq | 2; //enable aux ps/2 port irq12

  while(inb(0x64) & 2);
  outb(0x64, 0x60); // send command accept configuration
  while(inb(0x64) & 2);
  outb(0x60, cmq); // input configuration

  while(inb(0x64) & 2);
  outb(0x64, 0xa8); // enable aux ps/2 port

  while(inb(0x64) & 2);
  outb(0x64, 0xd4); //send next command to mouse
  while(inb(0x64) & 2);
  outb(0x60, 0xf4); // enable packets
  while((inb(0x64) & 1) != 1);
  while (inb(0x60) != 0xfa); // wait for ACK

  register_interrupt_handler(IRQ12, mouse_callback);
}
