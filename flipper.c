#include "flipper.h"

static int hasSetup = 0;

void FlipLazyBegin() {
    if (!hasSetup) {
        TRISBbits.RB6 = 0;
        TRISBbits.RB7 = 0;
    }

    hasSetup = 1;
}

void FlipA() {
    FlipLazyBegin();
    
    for (int i = 0; i < 5; ++i)
        RB7 = ~RB7;
}

void FlipB() {
    FlipLazyBegin();

    for (int i = 0; i < 5; ++i)
        RB6 = ~RB6;
}