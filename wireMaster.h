#ifndef WIRE_MASTER_H_
#define WIRE_MASTER_H_

#include "async.h"

extern int wireMasterEnabled;

void WireSend(int deviceId, unsigned char *data, int len, AsyncCallback_t callback);

//Data and info encoded as json (AsyncMessage) { data: message }
void WireGetString(int address, AsyncCallback_t callback);

//Setup I2C master mode
void WireBegin();

//Caled when SSPIF is enabled
void WireOnInterrupt();

#endif