#include "wireMaster.h"

static int wireMasterEnabled = 0;

//Store a sending information for ques
typedef struct {
    int deviceId;
    unsigned char data[100];
    int len;
    AsyncCallback_t callback;
} WireSend_t;

static WireSend_t wireSendA;
static WireSend_t wireSendB;
static WireSend_t wireSendC;
static WireSend_t wireSendD;

void WireSendInit() {
    //Not assigned
    wireSendA.deviceId = -1;
    wireSendB.deviceId = -1;
    wireSendC.deviceId = -1;
    wireSendD.deviceId = -1;
}

void WireSend(int deviceId, unsigned char *data, int len, AsyncCallback_t callback) {
    if (wireSendA.deviceId == -1) {
        wireSendA.deviceId = deviceId;
        memcpy(wireSendA.data, data, len);
        wireSendA.len = len;
        wireSendA.callback = callback;
    }

    if (wireSendB.deviceId == -1) {
        wireSendB.deviceId = deviceId;
        memcpy(wireSendB.data, data, len);
        wireSendB.len = len;
        wireSendB.callback = callback;
    }

    //Start I2C main
    SEN = 1;
}

void WireBegin() {
    wireMasterEnabled = 1;

    TRISC = 0b00011000; //RC3-RC4 INPUTS
    SSPM3 = 1; //set as I2C master
    SSPM2 = 0;
    SSPM1 = 0;
    SSPM0 = 0;
    SMP = 1; //disable slew rate control (running at 100khz)
    SSPEN = 1; //enable serial port

    SEN = 1; //create START condition
    SSPADD = 0x09; //set baud rate to 100khz
    SSPBUF = 0xA2; //buffer in number

    WireSendInit();
}

#define AFTER_START SSPSTATbits.S
#define AFTER_STOP SSPSTATbits.P
#define WE_GOT_ACK ACKSTAT

static int count = 0;

//#########################
//Helpers to handle I2C Master logic
void SendData() {
    if (count < 3) {
        SSPBUF = count;
        count = count + 1;
    } else {
        PEN = 1;
    }
}
//#########################

void WireOnInterrupt() {
    LA2 = ~LA2;
   // WCOL = 0;
    //#########################
    //Starting something
    //SSPBUF = 3;
    //#########################


    //#########################
    //Done
    //else if (AFTER_STOP) {

    //}
    //#########################
    

    //#########################
    //Load data
    //else {
    //    if (WE_GOT_ACK)
    //        SendData();
    //    else
    //        3 == 3;
    //}
    //#########################
}