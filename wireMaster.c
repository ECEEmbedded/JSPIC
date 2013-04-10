#include "wireMaster.h"

static int wireMasterEnabled = 0;

//Store a sending information for ques
typedef struct {
    int address;
    unsigned char data[100];
    int bytesSent; //How much have we sent?
    int len;
    AsyncCallback_t callback;
} WireSend_t;
static WireSend_t wireSendA;
static WireSend_t wireSendB;
static WireSend_t wireSendC;
static WireSend_t wireSendD;

#define AIsVacant (wireSendA.address == -1)
#define BIsVacant (wireSendB.address == -1)

//Keep track of what we are doing
#define Started 1
#define SentAddress 2
#define SentSomeData 3
#define Stopped 4
static int state;

//Who's being helped atm?
static WireSend_t *personBeingHelped;

void WireBegin() {
    wireMasterEnabled = 1;

    //Basic config
    TRISC3 = 1;
    TRISC4 = 1;
    SSPSTAT |= 0x80;
    SSPCON1 = 0x28;
    SSPADD = 0x0A;
    SSPIE = 1;

    //Clear for assignment later (que)
    wireSendA.address = -1;
    wireSendB.address = -1;
    wireSendC.address = -1;
    wireSendD.address = -1;
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
        wireSendA.bytesSent = 0;
        wireSendA.len = len;
        wireSendA.callback = callback;
    }

    else if (BIsVacant) {
        wireSendB.address = address;
        wireSendB.bytesSent = 0;
        memcpy(wireSendB.data, data, len);
        wireSendB.len = len;
        wireSendB.callback = callback;
    }
    //###############################################

    TryToHelp();
}

void WireOnInterrupt() {
     if (state == Started) {
         SSPBUF = personBeingHelped->address << 1;
         state = SentAddress;
     } else if (state == SentAddress || state == SentSomeData) {
         if (personBeingHelped->bytesSent < personBeingHelped->len) {
            SSPBUF = personBeingHelped->data[personBeingHelped->bytesSent];
            ++personBeingHelped->bytesSent;
            state = SentSomeData;
         } else {
             PEN = 1;
             state = Stopped;
         }
     } else if (state == Stopped) {
         Async(personBeingHelped->callback, NULL);
         personBeingHelped->address = -1;
         personBeingHelped = NULL;
         TryToHelp();
     }
}