#include "jspic.h"

void serialOut() {
    Sout('H');
}

void onSetup() {
    SetInterval(100, serialOut);
}

void onLoop() {
    
}