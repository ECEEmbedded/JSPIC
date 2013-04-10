#include "jspic.h"
#include "json.h"

void onFinishedI2C(){
    //Output over serial (Check out global.h for changing this)
    printf("I2C mesage was sent");
}

void master() {
    static char output[150];
    output[0] = 'h';
    output[1] = 'e';
    output[2] = 'l';
    output[3] = 0;

    //Send message and callback when finished
    WireSend(0x4F, output, 4, onFinishedI2C);
}

void onSetup() {
    //Called once in a lifetime
    TRISA = 0;

    //Setup I2C master
    WireBegin();

    //Call the master function every 100ms (Sends an I2C request)
    SetInterval(100, master);
}

void onLoop() {
    //Free While Loop
}