#include "string2.h"

void strcpy2(unsigned char *dest, const unsigned char *loc) {
    if (loc == NULL)
        return;
    
    while (*loc) {
        *dest = *loc;
        ++dest;
        ++loc;
    }
}
