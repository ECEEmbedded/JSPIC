#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <xc.h>
#include "interval.h"
#include "serial.h"
#include "jwire.h"
#include "wireMaster.h"

interrupt void onInterrupt() {

    //I2C
    if (SSPIF) {
        SSPIF = 0;

        if (jWireEnabled) {
            JWireOnInterrupt();
        }

        if (wireMasterEnabled) {
            WireOnInterrupt();
        }
    }
    
    //Millisecond timer
    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        OnIntervalInterrupt();
    }

    //Serial
    if (RCIF) {
        OnSerialInterrupt();
    }

    if (TXIF) {
        SerialWriteInterrupt();
    }

    //AsyncTick();
}

#endif