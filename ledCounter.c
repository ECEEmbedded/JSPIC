#include "ledCounter.h"
#include "interval.h"

void CountUp() {
    static int count = 0;

    LATA = (PORTA & 0xF8) | count;

    ++count;
    count = count % 8;
}

void LedCounterBegin() {
    TRISA = 0;
    
    SetInterval(250, CountUp);
}