#include "jspic.h"
#include "json.h"
#include "twitter.h"

void respond() {
    int output = TweetGetValue();

    Count(output);
}

void count() {
    static int counter = 0;
    ++counter;
    counter = counter % 8;
    TweetValue("@motor", "set", counter, respond);
}

void onSetup() {
    Count(0);
    TwitterWireMasterBegin();
    TwitterWireMasterAddSlave(0x4F, "@motor");

    SetInterval(100, count);
}

void onLoop() {
}