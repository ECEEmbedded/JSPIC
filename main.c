#include "jspic.h"
#include "json.h"
#include "twitter.h"

void call() {
    static int value = 0;
    value = JsonGetValue(AsyncMessage, "msg");
    printf("%d\n", value);
    TweetReturn("Thanks! :D");
}

void onSetup() {
    TwitterSignUp("@motor");
    TwitterWireSlaveBegin(0x4F);
    TwitterRegisterSubject("set", call);
}

void onLoop() {
}