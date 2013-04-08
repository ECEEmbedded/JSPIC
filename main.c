//#include "jspic.h"
//#include <xc.h>
#include "globals.h"
#include "interval.h"
#include "serial.h"
#include "json.h"
#include "jwire.h"
#include "wireMaster.h"

void serial(char *msg) {
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

void call() {

}

void myTimer() {
    //static char msg[1];
    //printf("tick tock...\n");
    //WireSend(3, msg, 1, call);

    
}

void gotJ() {
    static char world[20];
    //jsonGetString(AsyncMessage, "hello", world);
    printf("Pi is equal to %d and hello -> \n", jsonGetValue(AsyncMessage, "pi"));
}

void onRequest() {
    JWireRespond("Suck it this is a really long request not usually available on i2c");
}

void onSetup() {
    TRISA = 0;
    MacawBegin();
    SerialBegin();
    SetInterval(100, myTimer);

   // JWireBegin(0x4F);

    //JWireOnReceive(gotJ);
    //JWireOnRequest(onRequest);
    WireBegin();
}

void onLoop() {
}