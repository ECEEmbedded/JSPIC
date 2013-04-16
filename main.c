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
    TwitterSignUp("@master");
    TwitterWireMasterAddSlave(0x4F, "@motor");
    TwitterWireMasterBegin();
   // TwitterSerialBegin();

    SetInterval(400, count);
}

void onLoop() {
}