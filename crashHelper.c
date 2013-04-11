#include "crashHelper.h"

//Helps catch crashes on interrupts (Will go back to this)
static int IsCrashed = 0;

void Crash() {
    IsCrashed = 1;
    TRISA = 0;
    LA3 = 1;

    while (1);
}