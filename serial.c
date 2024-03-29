#include "serial.h"

#define MAX_MESSAGE_SIZE 150

//Buffer to store serial data
static char bufferSerial[MAX_MESSAGE_SIZE];
static int bufferPosition = 0;

//Default handler (Use SetSerialHandler to actually do something)
static AsyncCallback_t callback = 0;

void OnSerialInterrupt() {
    static char data;
    data = RCREG;

    //Error
    if (FERR | OERR) {
        CREN = 0;
        CREN = 1;
        return;
    }

    //EOF
    if (data == 0) {
        for (int i = 0; i < 10; ++i)
        LA3 = ~LA3;
        bufferSerial[bufferPosition] = 0;
        bufferPosition = 0;

        //Que the handler
        if (callback) {

            Async(callback, bufferSerial);
        }

        return;
     }

     //Put data in buffer
     bufferSerial[bufferPosition] = data;
     ++bufferPosition;
}

void SerialBegin() {
    //Inital setup
    SPEN = 1;
    TRISCbits.RC7 = 1;
    TRISCbits.RC6 = 1;

    //Configure transmission
    TX9 = 0;
    TXEN = 1;
    SYNC = 0;
    SENDB = 1;

    //Configure receiving
    CREN = 1;

    //Configure baud-rate to 57kbits/s
    BRGH = 1;
    BRG16 = 1;
    SPBRG = 103;

    //Enable interrupts
    TXIE = 0;
    RCIE = 1;
}

void OnSerial(AsyncCallback_t callbackToUse) {
    //Set callback
    callback = callbackToUse;
}

//Called by the main interrupt handler to write serial requests byte by byte
char writeBuffer[100];
static int writeBufferPosition = 0; //What position are we currently reading
static int writeBufferPositionOpen = 0; //What position are we currently writing
static int isDoneTransmitting = 0;

void SerialWrite(char *data) {
    if (writeBufferPositionOpen == 0) {
        strcpy(writeBuffer, data);
    } else {
        writeBuffer[writeBufferPositionOpen] = 1; //Continue character
        strcpy(&writeBuffer[writeBufferPositionOpen+1], data);
        ++writeBufferPositionOpen;
    }

    writeBufferPositionOpen = writeBufferPositionOpen + strlen(data);  //Points to null

    //Start a serial write request
    TXIE = 1;
    isDoneTransmitting = 0;
}

void SerialWriteInterrupt() {
    if (isDoneTransmitting) {
        TXIE = 0;
        return;
    }

    if (writeBuffer[writeBufferPosition] == 0) {
        TXREG = 0;
        writeBufferPosition = 0;
        writeBufferPositionOpen = 0;
        isDoneTransmitting = 1;
    //Continue character
    } else if (writeBuffer[writeBufferPosition] == 1) {
        TXREG = 0;
        ++writeBufferPosition;
   //Just write
   } else {
        TXREG = writeBuffer[writeBufferPosition];
        ++writeBufferPosition;
    }
}
