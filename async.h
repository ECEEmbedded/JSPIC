#ifndef ASYNC_H_
#define ASYNC_H_

#include "globals.h"
#include "string2.h"

extern char *AsyncMessage;

typedef void (*AsyncCallback_t)();

//Run this before using Async
void AsyncBegin();

//Que a function asynchronously
void Async(AsyncCallback_t callback, char *string);

//Main program loop should call this
void AsyncTick();

#endif