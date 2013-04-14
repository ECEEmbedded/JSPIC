#include "jspic.h"
#include "json.h"
#include "twitter.h"

static int value = 0;

//void call() {
//    //value = JsonGetValue(AsyncMessage, "msg");
//    //printf("%d\n", value);
//    TweetReturnValue(value);
//    value += 1;
//}

void call() {
    static int count = 0;
    if (count == 0)
        count = 1;
    else
        count = count << 1;

    TweetValue("#motor", "", count, NULL);

    count = count % 8;
}

void call2() {
    //static int key = 0;
    //key = JsonGetValue(AsyncMessage, "key");
    //printf("key: %d", key);

    static int count;
    count = JsonGetValue(AsyncMessage, "msg");
    LATA = (PORTA & 0xF8) | count;
    count = count % 8;
}

void onSetup() {
    TRISA = 0;
    //TwitterSignUp("@ir");
    TwitterSignUp("@motor");
    //TwitterRegisterHashtag("#motor", call2);
    //TwitterWireSlaveBegin(0x4F);
    //TwitterWireSlaveBegin(0x5F);
    //TwitterRegisterSubject("set", call);
    TwitterSerialBegin();



    SetInterval(150, call);
}

void onLoop() {
}