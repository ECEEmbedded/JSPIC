#include "wireMaster.h"

static int wireMasterEnabled = 0;

//Store a sending information for ques
typedef struct {
    int isSending;
    int address;
    unsigned char data[100];
    int bytesLeft; //How much have we sent?
    int len;
    AsyncCallback_t callback;
} WireSend_t;
static WireSend_t wireSendA;
static WireSend_t wireSendB;

#define AIsVacant (wireSendA.address == -1)
#define BIsVacant (wireSendB.address == -1)

//Keep track of what we are doing
#define Started 1
#define SentAddress 2
#define SentSomeData 3
#define Stopped 4
#define GotSomeData 5
#define WaitingForSomeData 6
static int state;

//Who's being helped atm?
static WireSend_t *personBeingHelped;

void WireBegin() {
    wireMasterEnabled = 1;

    //Basic config
    RCEN = 1;
    TRISC3 = 1;
    TRISC4 = 1;
    SSPSTAT |= 0x80;
    SSPCON1 = 0x28;
    SSPADD = 0x0A;
    SSPIE = 1;

    //Clear for assignment later (que)
    wireSendA.address = -1;
    wireSendB.address = -1;
    personBeingHelped = NULL;
}

void TryToHelp() {
    //If no one is being hepled
    if (!personBeingHelped) {
        //Find someone to help
        if (!AIsVacant) {
            personBeingHelped = &wireSendA;
        }
        else if (!BIsVacant) {
            personBeingHelped = &wireSendB;
        }

        //Only help someone if there is someone to help
        if (personBeingHelped) {
            //Start I2C main
            state = Started;
            SEN = 1;
        }
    }
}

void WireSend(int address, unsigned char *data, int len, AsyncCallback_t callback) {
    //Find a place to put the call
    //###############################################
    if (AIsVacant) {
        wireSendA.address = address;
        memcpy(wireSendA.data, data, len);
        wireSendA.bytesLeft = 0;
        wireSendA.len = len;
        wireSendA.callback = callback;
        wireSendA.isSending = 1;
    }

    else if (BIsVacant) {
        wireSendB.address = address;
        wireSendB.bytesLeft = 0;
        memcpy(wireSendB.data, data, len);
        wireSendB.len = len;
        wireSendB.callback = callback;
        wireSendB.isSending = 1;
    }
    //###############################################

    TryToHelp();
}

void WireGetString(int address, AsyncCallback_t callback) {
    //Find a place to put the call
    //###############################################
    if (AIsVacant) {
        wireSendA.address = address;
        wireSendA.len = 0;
        wireSendA.callback = callback;
        wireSendA.isSending = 0;
    }

    else if (BIsVacant) {
        wireSendB.address = address;
        wireSendB.len = 0;
        wireSendB.callback = callback;
        wireSendB.isSending = 0;
    }
    //###############################################

    TryToHelp();
}

void HandleSends() {
    //Send address
    if (state == Started) {
         SSPBUF = personBeingHelped->address << 1;
         state = SentAddress;
     //Send some data
     } else if (state == SentAddress || state == SentSomeData) {
         if (personBeingHelped->bytesLeft < personBeingHelped->len) {
            SSPBUF = personBeingHelped->data[personBeingHelped->bytesLeft];
            ++personBeingHelped->bytesLeft;
            state = SentSomeData;
         } else {
             PEN = 1;
             state = Stopped;
         }

     //Stop
     } else if (state == Stopped) {
         Async(personBeingHelped->callback, NULL);
         personBeingHelped->address = -1;
         personBeingHelped = NULL;
         TryToHelp();
     }
}

void HandleReads() {
    //Send address
    if (state == Started) {
         SSPBUF = personBeingHelped->address << 1 | 1;
         state = WaitingForSomeData;

     //Waiting for data
     } else if (state == WaitingForSomeData) {
         RCEN = 1;
         state = GotSomeData;
     //Got some data
     } else if (state == SentAddress || state == GotSomeData) {
         personBeingHelped->data[personBeingHelped->len] = SSPBUF;

         if (personBeingHelped->data[personBeingHelped->len] == 0) {
             PEN = 1;
             state = Stopped;
         } else {
             state = WaitingForSomeData;
             ACKEN = 1;
         }

         ++personBeingHelped->len;

     //Stop
     } else if (state == Stopped) {
         Async(personBeingHelped->callback, personBeingHelped->data);
         personBeingHelped->address = -1;
         personBeingHelped = NULL;
         TryToHelp();
     }
}

void WireOnInterrupt() {
    if (personBeingHelped->isSending)
        HandleSends();
    else
        HandleReads();
}