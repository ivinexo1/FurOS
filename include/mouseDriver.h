#ifndef MOUSE_DRIVER_H
#define MOUSE_DRIVER_H

#include <stdint.h>

void handle_mouse(void);

int get_mouse_x(void);
int get_mouse_y(void);
uint8_t get_mouse_buttons(void);

#endif // MOUSE_DRIVER_H