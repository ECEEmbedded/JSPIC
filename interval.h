#ifndef INTERVAL_H_
#define INTERVAL_H_

#include <xc.h>
#include "async.h"

//Setup interval for some milliseconds
void IntervalBegin(int ms);

//Called on the actual interval interrupt
void OnIntervalInterrupt();

//Configure the serial callback
void SetInterval(int delayInMs, void (*callbackToUse)());

#endif