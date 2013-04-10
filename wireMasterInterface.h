#ifndef WIRE_MASTER_INTERFACE
#define WIRE_MASTER_INTERFACE

#include "async.h"

void WireMasterInterfaceSend(char *message);
void WireMasterInterfaceOn(char *eventName, AsyncCallback_t callback);

#endif