#include "flipper.h"

static int hasSetup = 0;

void FlipLazyBegin() {
    if (!hasSetup) {
        PORTBbits.RB0 = 0;
        PORTBbits.RB1 = 0;
    }

    hasSetup = 1;
}

void FlipA() {
    FlipLazyBegin();
    
    for (int i = 0; i < 5; ++i)
        LB0 = ~LB0;
}

void FlipB() {
    FlipLazyBegin();

    for (int i = 0; i < 5; ++i)
        LB1 = ~LB1;
}