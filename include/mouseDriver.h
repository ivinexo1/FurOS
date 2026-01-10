#pragma once
#include "../include/isr.h"

static void parse_mouse_packet(uint8_t p[3]);
void mouse_callback(registers_t *regs);
void init_mouse();