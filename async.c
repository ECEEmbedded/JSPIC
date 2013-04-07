#include "async.h"

#define MAX_MESSAGE_SIZE 200

//Callback structure that holds the info for the que
typedef struct CALLBACK_T {
    void (*function)(char *);
    char string[MAX_MESSAGE_SIZE];
} Callback_t;

//Callback holders (DO NOT TRY TO USE AN ARRAY, XC8 FLIPS SHIT)
static Callback_t callbackA;
static Callback_t callbackB;
static Callback_t callbackC;
static Callback_t callbackD;
static Callback_t callbackE;
static Callback_t callbackF;

void Async(void (*function)(char *), char *string) {
    //Load an open callback
    if (callbackA.function == NULL) {
        strcpy2(callbackA.string, string);
        callbackA.function = function;
    } else if (callbackB.function == NULL) {
        strcpy2(callbackB.string, string);
        callbackB.function = function;
    } else if (callbackC.function == NULL) {
        strcpy2(callbackC.string, string);
        callbackC.function = function;
    } else if (callbackD.function == NULL) {
        strcpy2(callbackD.string, string);
        callbackD.function = function;
    } else if (callbackE.function == NULL) {
        strcpy2(callbackE.string, string);
        callbackE.function = function;
    } else if (callbackF.function == NULL) {
        strcpy2(callbackF.string, string);
        callbackF.function = function;
    }
}

void AsyncTick() {
    //Run any functions that need to be run
    if (callbackA.function) {
        callbackA.function(callbackA.string);
    }

    if (callbackB.function) {
        callbackB.function(callbackB.string);
    }

    if (callbackC.function) {
        callbackC.function(callbackC.string);
    }

    if (callbackD.function) {
        callbackD.function(callbackD.string);
    }

    if (callbackE.function) {
        callbackE.function(callbackE.string);
    }

    if (callbackF.function) {
        callbackF.function(callbackF.string);
    }

    //Reset all function pointers
    AsyncBegin();
}

void AsyncBegin() {
    //Clear all the functions to resting
    callbackA.function = NULL;
    callbackB.function = NULL;
    callbackC.function = NULL;
    callbackD.function = NULL;
    callbackE.function = NULL;
    callbackF.function = NULL;

}