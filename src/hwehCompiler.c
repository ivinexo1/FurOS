#include "../include/hwehCompiler.h"
#include "../include/terminal.h"

// lexer (turn source code into tokens)
char src[1024] = "[START] $ src [END]";
char letter[1024];
char word[256];
int j = 0;

for(int i = 0; i < 1024; i++) {
    if (src[i] == ' ' || src[i] == '\0') {
        letter[i] = '\0'; // end of token
        word[j] = letter;
        j += 1;
        break;
    }
    letter[i] = src[i];
}
printString(letter);