//#include "jspic.h"
//#include <xc.h>
#include "globals.h"
#include "interval.h"
#include "serial.h"
#include "json.h"

void serial(char *msg) {
    LA0 = ~LA0;
    //printf(msg);
    if (msg[0] == 'm')
        LA0 = ~LA0;

    if (msg[1] == 'a')
        LA0 = ~LA0;

    if (msg[2] == 't')
        LA0 = ~LA0;

    if (msg[3] == '\n')
        LA0 = ~LA0;

    if (msg[4] == 0)
        LA0 = ~LA0;
}

void myTimer() {
    static char seo[200];


    jsonNew(seo);
    jsonSetString(seo, "call", "get/ir1 my name!");
    jsonSetValue(seo, "key0", 3);
    jsonSetValue(seo, "key1", 4);
    jsonSetValue(seo, "key2", 5);
    jsonSetValue(seo, "key3", 6);
    jsonSetValue(seo, "key4", 7);


    SerialWrite(seo);
}

void onSetup() {
    TRISA = 0;
    MacawBegin();
    SerialBegin();
    SetInterval(70, myTimer);;;;;;;;

}

void onLoop() {
    //LA0 = ~LA0;
    //LA1 = ~LA1;
    LA2 = ~LA2;
}