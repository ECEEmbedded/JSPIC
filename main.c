#include "jspic.h"
#include "json.h"
#include "twitter.h"

void call2() {
    
}

void call() {
    static char data[50];
    sprintf(data, "hello world! this is the shit!\n");
    WireSend(0x4F, data, strlen(data)+1, call2);
}

void onSetup() {
    //Setup I2C master
    WireBegin();

    SetInterval(100, call);
}

void onLoop() {
}