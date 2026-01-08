#include "../include/mouseDriver.h"
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