#include "async.h"
#include "crashHelper.h"

#define MAX_MESSAGE_SIZE 100

//Callback holders (DO NOT TRY TO USE AN ARRAY, XC8 FLIPS SHIT)
AsyncCallback_t callbackA;
static char callbackAString[MAX_MESSAGE_SIZE];

AsyncCallback_t callbackB;
static char callbackBString[MAX_MESSAGE_SIZE];

AsyncCallback_t callbackC;
static char callbackCString[MAX_MESSAGE_SIZE];

static char *AsyncMessage = 0;

void Async(AsyncCallback_t function, char *string) {
    //Load an open callback
    if (callbackA == 0) {
        strcpy2(callbackAString, string);
        callbackA = function;
    } else if (callbackB == 0) {
        strcpy2(callbackBString, string);
        callbackB = function;
    } else if (callbackC == 0) {
        strcpy2(callbackCString, string);
        callbackC = function;
    } else {
        Crash();
    }
}

void AsyncTick() {
    //Run any functions that need to be run
    if (callbackA) {
        AsyncMessage = callbackAString;
        callbackA();
    }

    if (callbackB) {
        AsyncMessage = callbackBString;
        callbackB();
    }

    if (callbackC) {
        AsyncMessage = callbackCString;
        callbackC();
    }

    //Reset all function pointers
    AsyncBegin();
}

void AsyncBegin() {
    //Clear all the functions to resting
    callbackA = 0;
    callbackB = 0;
    callbackC = 0;
}