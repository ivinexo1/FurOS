#include <stdint.h>
#include "../include/mouseDriver.h"
#include "../include/ports.h"

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
}

void handle_mouse(void) {
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