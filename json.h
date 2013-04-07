#ifndef JSON_H_
#define JSON_H_

#include "string2.h"

void jsonNew(char* string);     // Add braces; basically, the object you are
// going to add your data too.

void jsonSetString(char* input, char* key, char* string);
void jsonSetValue(char* input, char* key, int value);

int jsonGetString(char* input, char* key, char* string);
int jsonGetValue(char* input, char* key);
#endif