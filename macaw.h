#ifndef MACAW_H_
#define MACAW_H_

#include "globals.h"

void MacawBegin();

//Used for printf utility
#ifdef MACAW_CONSOLE
void putch(char data);
#endif

#endif