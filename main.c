#include "jspic.h"
#include "json.h"
#include "twitter.h"

void counter() {
    static int n = 0;
    Count(n);
    ++n;
}

void onSetup() {
    TwitterSignUp("@master");
    TwitterSerialBegin();
    TwitterWireMasterBegin();
    TwitterWireMasterAddSlave(0x4F, "@ir");
    //SetInterval(80, call);
}

void onLoop() {
    counter();
}