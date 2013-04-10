#include "jspic.h"
#include "json.h"

void blah (){
for (int i = 0; i < 10; ++i)
    LA1 = ~LA1;
}

void master() {
    static char output[150];
    jsonNew(output);
    static int count = 0;
    ++count;
    jsonSetString(output, "hello", "wod");
    jsonSetValue(output, "oeu", count);
    WireSend(0x4F, output, strlen(output), blah);
    WireSend(0x4F, output, strlen(output), blah);
}

void onSetup() {
    TRISA = 0;
    WireBegin();
    SetInterval(15, master);
     static char data[1];
    //WireSend(0x4F, data, 1, blah);
}

void onLoop() {
}