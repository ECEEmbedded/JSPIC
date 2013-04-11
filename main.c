#if 1

//SLAVE

#include "jspic.h"
#include "json.h"
#include "twitter.h"
void call() {
    static char name[100];
    strcpy(name, "shitface");
    static int value = 0;
    JsonNew(AsyncMessage);
    JsonSetString(AsyncMessage, "name", name);
    JsonSetValue(AsyncMessage,"sensor", value);
    ++value;

    JWireRespond(AsyncMessage);
}

void onSetup() {
    SerialBegin();

    //SetInterval(100, call);
    JWireBegin(0x4F);
    JWireOnRequest(call);;
}

void onLoop() {

}
#endif

#if 0

//MASTER

#include "jspic.h"
#include "json.h"
#include "twitter.h"

void call2() {
    static char message[50];
    int value = JsonGetValue(AsyncMessage, "sensor");
    JsonGetString(AsyncMessage, "name", message);
    sprintf(AsyncMessage, "%s %d\n", message, value);
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
    SetInterval(20, call);
}

void onLoop() {
}

#endif