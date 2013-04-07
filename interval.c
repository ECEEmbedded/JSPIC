#include "interval.h"

//Store function pointer and delay time
typedef struct INTERVAL_CALLBACK_T {
    void (*function)();
    short delay;
} IntervalCallback_t;

static IntervalCallback_t callbackA;
static IntervalCallback_t callbackB;
static IntervalCallback_t callbackC;

//Handle interval timings in userland
void IntervalTick() {
    static int time = 0;

    if (callbackA.function && (time % callbackA.delay == 0))
        callbackA.function();

    if (callbackB.function && (time % callbackB.delay == 0))
        callbackB.function();

    if (callbackC.function && (time % callbackC.delay == 0))
        callbackC.function();

     ++time;
}

void OnIntervalInterrupt() {
    //Que the handlers
    Async(IntervalTick, NULL);
}

void SetInterval(int delayInMs, void (*callbackToUse)()) {
    //Lazy load
    static char hasLoaded = 0;
    if (!hasLoaded) {
        //Timer 0 interrupt at 1 second
        T0CONbits.TMR0ON = 1;
        T0CONbits.T08BIT = 1; //Divide timer by 256
        T0CONbits.T0CS = 0; //Use internal clock for timing
        T0CONbits.PSA = 0;
        T0CONbits.T0PS = 2;
        INTCONbits.TMR0IE = 1;

        callbackA.function = NULL;
        callbackB.function = NULL;
        callbackC.function = NULL;
        
        hasLoaded = 1;
    }

    if (callbackA.function == NULL) {
        callbackA.function = callbackToUse;
        callbackA.delay = delayInMs;
    } else if (callbackB.function == NULL) {
        callbackB.function = callbackToUse;
        callbackB.delay = delayInMs;
    } else if (callbackC.function == NULL) {
        callbackC.function = callbackToUse;
        callbackC.delay = delayInMs;
    }
}