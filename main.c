#include "jspic.h"
#include "json.h"
#include "twitter.h"

void onFinishedI2C(){
    //Output over serial (Check out global.h for changing this)
    printf("I2C mesage was sent");
}

void on() {
    
}

void call() {
    static int count = 0;
 
    static char message[200];
    sprintf(message, "%s %d\n", AsyncMessage, count);
    ++count;
    SerialWrite(message);
}

void onSetup() {
    Crash();
    SerialBegin();
    
    //Called once in a lifetime
    TRISA = 0;

    //Setup I2C master
    WireBegin();

   // TwitterSignUp("@master");
}

void onLoop() {
    //Free While Loop
}