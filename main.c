#include "jspic.h"
#include "json.h"
#include "twitter.h"

void count() {
    static int counter = 0;
    ++counter;

    Count(counter);
}

void onSetup() {
}

void onLoop() {
    count();
}