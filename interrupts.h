#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <xc.h>
#include "interval.h"
#include "serial.h"
#include "jwire.h"
#include "wireMaster.h"

interrupt void onInterrupt();

#endif