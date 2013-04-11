#include "twitter.h"

//Use this for string manipulations, just free space...
static char tempBuffer[150];

//Used to maintain 2-way sessions (Callbacks)
typedef struct SESSION {
    int key;
    AsyncCallback_t callback;
} Session;
static Session sessionA;
static Session sessionB;
static Session sessionC;
static Session sessionD;
#define SessionAVacant (sessionA.key == -1)
#define SessionBVacant (sessionB.key == -1)
#define SessionCVacant (sessionB.key == -1)
#define SessionDVacant (sessionB.key == -1)

//Manage callbacks
typedef struct HASHTAG {
    char hashtag[20];
    AsyncCallback_t callback;
} Hashtag;

static Hashtag hashtagA;
static Hashtag hashtagB;
static Hashtag hashtagC;

//Modify these for your platforms specific needs
//####################################################################

//###############################################################
//Slave Wire#########
static int TwitterWireSlaveEnabled = 0;

//Add Twitter to I2C Slave
void TwitterWireSlaveBegin(int id) {
    JWireBegin(id);
    JWireOnReceive(TwitterOnReceive);

    TwitterWireSlaveEnabled = 1;
}
//###############################################################

//###############################################################
//Master Wire#########
static int TwitterWireMasterEnabled = 0;

void TwitterWireMasterBegin() {
    WireBegin();
}

static int wireSlaveA = 0;
static int wireSlaveB = 0;
static int wireSlaveC = 0;

void TwitterWireMasterAddSlave(int id) {
    if (wireSlaveA == 0)
        wireSlaveA = id;
    else if (wireSlaveB == 0)
        wireSlaveB = id;
    else if (wireSlaveC == 0)
        wireSlaveC = id;
    else
        Crash(3);
}
//###############################################################

//Global multicast to all interfaces
void TwitterSendToAll(char *message) {
    if (TwitterWireSlaveEnabled) {
    } else if (TwitterWireMasterEnabled) {
        //Multicast to slaves
        if (wireSlaveA)
            WireSend(wireSlaveA, message, strlen(message)+1, NULL);
        if (wireSlaveB)
            WireSend(wireSlaveB, message, strlen(message)+1, NULL);
        if (wireSlaveC)
            WireSend(wireSlaveC, message, strlen(message)+1, NULL);
    }
}

//Register this callback function for incomming messages
void TwitterOnReceive() {
    char *message = AsyncMessage;
    
    //Get the 'to' field of this message
    JsonGetString(message, "to", tempBuffer);

    //Hashtag multicast
    if (tempBuffer[0] == '#') {
        //Is there any callbacks registered to this message?
        if (hashtagA.callback && !strcmp(hashtagA.hashtag, message)) {
            hashtagA.callback();
        } else if (hashtagB.callback && !strcmp(hashtagB.hashtag, message)) {
            hashtagB.callback();
        } else if (hashtagC.callback && !strcmp(hashtagC.hashtag, message)) {
            hashtagC.callback();
        }

        //Send message back if needed
        TwitterSendToAll(message);
    }
}
//####################################################################

static char name[20];
void TwitterSignUp(char *nameToUse) {
    strcpy(name, nameToUse);

    //Clear sessions
    sessionA.key = -1;
    sessionB.key = -1;
    sessionC.key = -1;
    sessionD.key = -1;

    //Clear hashtag
    hashtagA.callback = 0;
    hashtagB.callback = 0;
    hashtagC.callback = 0;
}

void Tweet(char *subject, char *to, char *message, AsyncCallback_t callback) {
    //Used for tracking potentional 2-way callback sessions
    static int sessionKey = 0;
    
    //Hashtag (Multicast message)
    if (to[0] == '#') {
        JsonNew(tempBuffer);
        JsonSetString(tempBuffer, "to", to);
        JsonSetString(tempBuffer, "from", name);
        JsonSetString(tempBuffer, "subject", subject);
        JsonSetString(tempBuffer, " msg", message);

        //Store session if a callback is registered
        if (callback) {
            //Just circular loop through available sessions
            static int nextSession = 0;

            //Load into the next vacant session
            if (nextSession == 0) {
                sessionA.callback = callback;
                sessionA.key = sessionKey;
            } else if (nextSession == 1) {
                sessionB.callback = callback;
                sessionB.key = sessionKey;
            } else if (nextSession == 2) {
                sessionC.callback = callback;
                sessionC.key = sessionKey;
            } else if (nextSession == 3) {
                sessionD.callback = callback;
                sessionD.key = sessionKey;
            }

            ++nextSession;
            nextSession = nextSession % 4;  //Rollback to first when done

            JsonSetValue(tempBuffer, "key", sessionKey);
        }

        //Multicast message
        TwitterSendToAll(tempBuffer);
    }

    ++sessionKey;
}

void TwitterRegisterHashtag(char *hashtag, AsyncCallback_t callback) {
    if (!hashtagA.callback) {
        hashtagA.callback = callback;
        strcpy2(hashtagA.hashtag, hashtag);
    } else if (!hashtagB.callback) {
        hashtagB.callback = callback;
        strcpy2(hashtagB.hashtag, hashtag);
    } else if (!hashtagC.callback) {
        hashtagC.callback = callback;
        strcpy2(hashtagC.hashtag, hashtag);
    } else {
        Crash(2);
    }
}