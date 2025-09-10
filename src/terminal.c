#include "../include/vga.h"
#include "../include/terminal.h"
#include <stdint.h>

uint32_t current_X = 0;
uint32_t current_Y = 0;
uint32_t fgcolor = 0xffffff;
uint32_t bgcolor = 0x0000ff;
uint8_t buffer[475];


int printChar(uint8_t letter){
  buffer[current_X + current_Y * 25] = letter;
  if (letter == ' ') {
    current_X++;
  } else if (letter == '\n') {
    current_X = 0;
    current_Y++;
  } else {
    putChar(current_X * 40, current_Y * 40, fgcolor, bgcolor, letter);
    current_X++;
  }
  if (current_Y > 18) {
    for (int i = 0; i < 25; i++) {
      for (int j = 0; j < 19; j++) {
        if (j < 18) {
          buffer[i + j*25] = buffer[(i + j*25) + 25];
          putChar(i * 40, j * 40, fgcolor, bgcolor, buffer[i + j*25]);
        } else {
          buffer[i + j*25] = 0;
          putChar(i * 40, j * 40, fgcolor, bgcolor, buffer[i + j*25]);
        }

      }
    }
    current_X = 0;
    current_Y = 18;
  }
  if (current_X >= 25) {
    current_X = 0;
    current_Y++;
  }
  return 0;
}
