#include "../include/shell.h"
#include "../include/vga.h"
#include "../include/ports.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../include/keyboard.h"
#include "../include/string.h" // nemame standard lib
#include "../include/terminal.h"

void shell() {
    int wlength = 0;
    char word[256];
    int words_count = 0;
    char* words[256];
    char command[256];
    char letter[256];
    for(int i = 0; i < 256; i++) { letter[i] = keybuffer[i]; }
    // find length of the word [until whitespace]
    while (keybuffer[wlength] == ' ' || keybuffer[wlength] == '\n' || keybuffer[wlength] == '\0' || keybuffer[wlength] == '\t') {
        wlength++;
    }
    // keybuffer => word
    for (int i = 0; i < wlength; i++) {
        word[i] = keybuffer[i];
    }
    word[wlength] = '\0';
    words[words_count] = word;
    words_count++;
    for (int i = 0; i < wlength; i++) {
        word[i] = '\0';
    }

    // now multiple words D:
    int j = 0;
    words_count = 0;
    while (keybuffer[j] != '\0') {
        // skip whitespace
        while (keybuffer[j] == ' ' || keybuffer[j] == '\n' || keybuffer[j] == '\t') j++;
        if (keybuffer[j] == '\0') break;
        words[words_count++] = &keybuffer[j];
        // move to end of word
        while (keybuffer[j] != ' ' && keybuffer[j] != '\n' && keybuffer[j] != '\t' && keybuffer[j] != '\0') j++;
        if (keybuffer[j] != '\0') {
            keybuffer[j] = '\0';
            j++;
        }
    }

    if (strcmp2(words[0], "sys") == 0) {
        if (strcmp2(words[1], "help") == 0) {
            printString("Available commands:\n");
            printString(" sys\n");
            printString("    help\n");
            printString("    echo\n");
            printString("    clear\n"); // (not implemented)
            printString("    shutdown\n"); // (not implemented)
            printString("    debug\n\n"); // (not implemented)
        }
        if (strcmp2(words[1], "echo") == 0) {
            for(int i = 2; i < words_count; i++) {
                printString(words[i]);
                if (i < words_count - 1) {
                    printString(" ");
                }
            }
            printString("\n");
        }
    }
    if(strcmp2(words[0], "debug") == 0) {
        if(strcmp2(words[1], "mouse") == 0) {
            // render image as background and show mouse
            // + refresh screen maybe 30 or 60hz
            printString("mouse debug\n");
        }
    } 
}