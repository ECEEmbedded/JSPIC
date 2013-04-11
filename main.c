#include "jspic.h"
#include "json.h"
#include "twitter.h"

void call2() {
    static char message[50];
    int value = JsonGetValue(AsyncMessage, "sensor");
    JsonGetString(AsyncMessage, "name", message);
    sprintf(AsyncMessage, "%s\n", message);
    SerialWrite(AsyncMessage);
}

void call() {
    WireGetString(0x4F, call2);
}

void hello() {
    
}

void onSetup() {
    //Setup I2C master
    WireBegin();
    SerialBegin();
    SetInterval(40, call);

    TwitterSignUp("@sotownesnd");
    TwitterAddWireSlave(0x4F);
    TwitterRegisterHashtag("#hello", hello);
}

void onLoop() {
}