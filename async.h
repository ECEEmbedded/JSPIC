#ifndef ASYNC_H_
#define ASYNC_H_

#include "globals.h"

typedef void (*AsyncCallback_t)(char *);

//Run this before using Async
void AsyncBegin();

//Que a function asynchronously
void Async(void (*function)(char *), char *string);

//Main program loop should call this
void AsyncTick();

#endif