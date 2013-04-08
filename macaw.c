#include "macaw.h"

void MacawBegin() {
    //Use port B to transfer paralell data
    TRISB = 0;
}

#ifdef MACAW_CONSOLE
void putch(char data) {
    //Ignore MSB because it is the clock bit, should work with all ascii characters
    LATB = data;

    //Clock
    RB7 = 1;
    RB7 = 0;

    __delay_us(35);
}
#endif