#include "interrupts.h"

interrupt void onInterrupt() {

    //I2C
    //###########################################
    if (SSPIF) {
        SSPIF = 0;

        //I2C Slave (J-Wire Implementation)
        if (jWireEnabled) {
            JWireOnInterrupt();
        }

        //I2C Master
        if (wireMasterEnabled) {
            WireOnInterrupt();
        }
    }
    //###########################################

    //Millisecond timer
    //###########################################
    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        OnIntervalInterrupt();
    }
    //###########################################

    //Serial (JSerial Implementation)
    //###########################################
    if (RCIF) {
        OnSerialInterrupt();
    }

    if (TXIF) {
        SerialWriteInterrupt();
    }
    //###########################################

    AsyncTick();

    //Catch crashes overriden by interrupts if possible
    if (IsCrashed) {
        Crash();
    }
}