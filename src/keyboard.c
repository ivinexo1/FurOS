#include "../include/keyboard.h"
#include "../include/ports.h"
#include "../include/isr.h"
#include "../include/vga.h"
#include "../include/terminal.h"
#include "../include/shell.h"

char keybuffer[MAX_SIZE];
int shift_pressed = 0; // 0 - shift not pressed, 32 - shift pressed
int keybuff_pointer = 0;

void print_letter(uint8_t scancode) {
    switch (scancode) {
        case 0x0:
//          //printString("ERROR");
          break;
        case 0x1:
          // ESC
          break;
        case 0x2:
          printChar(shift_pressed ? '!' : '1');
          keybuffer[keybuff_pointer] = '1' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x3:
          printChar(shift_pressed ? '@' : '2');
          keybuffer[keybuff_pointer] = '2' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x4:
          printChar(shift_pressed ? '#' : '3');
          keybuffer[keybuff_pointer] = '3' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x5:
          printChar(shift_pressed ? '$' : '4');
          keybuffer[keybuff_pointer] = '4' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x6:
          printChar(shift_pressed ? '%' : '5');
          keybuffer[keybuff_pointer] = '5' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x7:
          printChar(shift_pressed ? '^' : '6');
          keybuffer[keybuff_pointer] = '6' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x8:
          printChar(shift_pressed ? '&' : '7');
          keybuffer[keybuff_pointer] = '7' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x9:
          printChar(shift_pressed ? '*' : '8');
          keybuffer[keybuff_pointer] = '8' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x0A:
          printChar(shift_pressed ? '(' : '9');
          keybuffer[keybuff_pointer] = '9' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x0B:
          printChar(shift_pressed ? ')' : '0');
          keybuffer[keybuff_pointer] = '0' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x0C:
          printChar(shift_pressed ? '_' : '-');
          keybuffer[keybuff_pointer] = '-' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x0D:
          printChar(shift_pressed ? '+' : '=');
          keybuffer[keybuff_pointer] = '=' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x0E:
          // BACKSPACE
          if (keybuffer[0] != 0) {
            printChar('\r');
            keybuff_pointer--;
            keybuffer[keybuff_pointer] = 0;
          }
          break;
        case 0x0F:
//          //printString("\t");
          break;
        case 0x10:
          printChar(shift_pressed ? 'Q' : 'q');
          keybuffer[keybuff_pointer] = 'q' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x11:
          printChar(shift_pressed ? 'W' : 'w');
          keybuffer[keybuff_pointer] = 'w' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x12:
          printChar(shift_pressed ? 'E' : 'e');
          keybuffer[keybuff_pointer] = 'e' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x13:
          printChar(shift_pressed ? 'R' : 'r');
          keybuffer[keybuff_pointer] = 'r' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x14:
          printChar(shift_pressed ? 'T' : 't');
          keybuffer[keybuff_pointer] = 't' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x15:
          printChar(shift_pressed ? 'Y' : 'y');
          keybuffer[keybuff_pointer] = 'y' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x16:
          printChar(shift_pressed ? 'U' : 'u');
          keybuffer[keybuff_pointer] = 'u' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x17:
          printChar(shift_pressed ? 'I' : 'i');
          keybuffer[keybuff_pointer] = 'i' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x18:
          printChar(shift_pressed ? 'O' : 'o');
          keybuffer[keybuff_pointer] = 'o' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x19:
          printChar(shift_pressed ? 'P' : 'p');
          keybuffer[keybuff_pointer] = 'p' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x1A:
          printChar(shift_pressed ? '{' : '[');
          keybuffer[keybuff_pointer] = '[' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x1B:
          printChar(shift_pressed ? '}' : ']');
          keybuffer[keybuff_pointer] = ']' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x1C:
          // ENTER
          printChar('\n');
          // if(keybuffer[0] != 0) { parsedShell(); }
          printString("SHELL> ");
          for(int x = 0; x < MAX_SIZE; x++) keybuffer[x] = 0;
          keybuff_pointer = 0;
          break;
        case 0x1D:
          // LCTRL ~ shortcuts
          break;
        case 0x1E:
          printChar(shift_pressed ? 'A' : 'a');
          keybuffer[keybuff_pointer] = 'a' - shift_pressed;
          keybuff_pointer++;       
          break;
        case 0x1F:
          printChar(shift_pressed ? 'S' : 's');
          keybuffer[keybuff_pointer] = 's' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x20:
          printChar(shift_pressed ? 'D' : 'd');
          keybuffer[keybuff_pointer] = 'd' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x21:
          printChar(shift_pressed ? 'F' : 'f');
          keybuffer[keybuff_pointer] = 'f' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x22:
          printChar(shift_pressed ? 'G' : 'g');
          keybuffer[keybuff_pointer] = 'g' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x23:
          printChar(shift_pressed ? 'H' : 'h');
          keybuffer[keybuff_pointer] = 'h' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x24:
          printChar(shift_pressed ? 'J' : 'j');
          keybuffer[keybuff_pointer] = 'j' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x25:
          printChar(shift_pressed ? 'K' : 'k');
          keybuffer[keybuff_pointer] = 'k' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x26:
          printChar(shift_pressed ? 'L' : 'l');
          keybuffer[keybuff_pointer] = 'l' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x27:
          printChar(shift_pressed ? ':' : ';');
          break;
        case 0x28:
          printChar(shift_pressed ? '"' : '\'');
          break;
        case 0x29:
          printChar(shift_pressed ? '~' : '`');
          break;
        case 0x2A:
          // LSHIFT
          shift_pressed = 32;
          break;
        case 0x2B:
          //printString("\nI");
          for(int x = 0; x < MAX_SIZE; x++) printChar(keybuffer[x]);
          //printString("I");
          break;
        case 0x2C:
          printChar(shift_pressed ? 'Z' : 'z');
          keybuffer[keybuff_pointer] = 'z' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x2D:
          printChar(shift_pressed ? 'X' : 'x');
          keybuffer[keybuff_pointer] = 'x' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x2E:
          printChar(shift_pressed ? 'C' : 'c');
          keybuffer[keybuff_pointer] = 'c' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x2F:
          printChar(shift_pressed ? 'V' : 'v');
          keybuffer[keybuff_pointer] = 'v' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x30:
          printChar(shift_pressed ? 'B' : 'b');
          keybuffer[keybuff_pointer] = 'b' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x31:
          printChar(shift_pressed ? 'N' : 'n');
          keybuffer[keybuff_pointer] = 'n' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x32:
          printChar(shift_pressed ? 'M' : 'm');
          keybuffer[keybuff_pointer] = 'm' - shift_pressed;
          keybuff_pointer++;
          break;
        case 0x33:
          printChar(shift_pressed ? '<' : ',');
          keybuffer[keybuff_pointer] = ',' - shift_pressed;
          keybuff_pointer++;
        case 0x34:
          printChar(shift_pressed ? '>' : '.');
          keybuffer[keybuff_pointer] = '.' - shift_pressed;
          keybuff_pointer++;
        case 0x35:
          printChar(shift_pressed ? '?' : '/');
          keybuffer[keybuff_pointer] = '/' - shift_pressed;
          keybuff_pointer++;
        case 0x36:
          // RSHIFT
          shift_pressed = 32;
          break;
        case 0x37:
          //printString("*"); // Numpad multiply
          keybuffer[keybuff_pointer] = '*';
          keybuff_pointer++;
          break;
        case 0x38:
          // LALT
          break;
        case 0x39:
            printChar(' ');
            keybuffer[keybuff_pointer] = ' ';
            keybuff_pointer++;
            break;
        default:
            // 'keuyp' event corresponds to the 'keydown' + 0x80
            // it may still be a scancode we haven't implemented yet, or
            // maybe a control/escape sequence 
            if (scancode <= 0x7f) {
                //printString("<?>"); // unknown key down. � neni supported, dost smutne
            } else if (scancode <= 0x39 + 0x80) {
                if((scancode == 0x2A + 0x80) || (scancode == 0x36 + 0x80)) {
                    shift_pressed = 0; // reset shift on key up
                }
                ////printString("key up ");
                //print_letter(scancode - 0x80);
            } else ////printString(""); // unknown key up
            break;
      }
  if(keybuff_pointer == MAX_SIZE){
    //printString(" Max size is 256 cannot store bigger strings in buffer  ");
    for(int x = 0; x < MAX_SIZE; x++) keybuffer[x] = 0;
    keybuff_pointer = 0;
  }

}

void keyboard_callback(registers_t *regs) {
    uint8_t scancode = inb(0x60);
    print_letter(scancode);
    //printChar('\n'); nechces radsej dat ze new line iba ked sa stlaci enter?
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}

