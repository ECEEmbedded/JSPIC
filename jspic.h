#ifndef JSPIC_H_
#define JSPIC_H_

#include <xc.h>
#include "macaw.h"
#include "async.h"
#include "jwire.h"
#include "interrupts.h"
#include "interval.h"
#include "serial.h"
#include "globals.h"
#include "superSimpleSerial.h"
#include "ledCounter.h"

void onSetup();
void onLoop();

#endif