#include "twitter.h"

//Use this for string manipulations, just free space...
static char tempBuffer[150];
static char tempBuffer2[150];

//Used to maintain 2-way sessions (Callbacks)
typedef struct SESSION {
    int key;
    AsyncCallback_t callback;
} Session;
static Session sessionA;
static Session sessionB;
#define SessionAVacant (sessionA.key == -1)
#define SessionBVacant (sessionB.key == -1)

//Manage hashtag callbacks
typedef struct HASHTAG {
    char hashtag[20];
    AsyncCallback_t callback;
} Hashtag;
static Hashtag hashtagA;
static Hashtag hashtagB;
static Hashtag hashtagC;

//Manage regular subject based callbacks
typedef struct SUBJECT {
    char subject[20];
    AsyncCallback_t callback;
} Subject;
static Subject subjectA;
static Subject subjectB;

//My Name @example
static char name[20];


//Modify these for your platforms specific needs
//####################################################################

//###############################################################
//Slave Wire#########
static int TwitterWireSlaveEnabled = 0;

//Wait around to be pulled and send a slave message
static char twitterWireSlaveMessageToSend[150];;

//Master is requesting something
void TwitterWireSlaveOnRequest() {
    printf("Got a slave request!");
}

//Add Twitter to I2C Slave
void TwitterWireSlaveBegin(int id) {
    JWireBegin(id);
    JWireOnReceive(TwitterOnReceive);
    JWireOnRequest(TwitterWireSlaveOnRequest);

    //Clear message
    twitterWireSlaveMessageToSend[0] = 0;

    TwitterWireSlaveEnabled = 1;
}
//###############################################################

//###############################################################
//Master Wire#########
typedef struct WIRE_SLAVE {
    int id;
    char name[20];
} WireSlave;
static WireSlave wireSlaveA;
static WireSlave wireSlaveB;
static WireSlave wireSlaveC;

static int TwitterWireMasterEnabled = 0;

//Fake slave requests by polling
void TwitterWireMasterRequestSlave() {
    static int current = 0;

    if (current == 0 && wireSlaveA.id != 0) {
        WireGetString(wireSlaveA.id, TwitterOnReceive);
    } else if (current == 1 && wireSlaveB.id != 0) {
        WireGetString(wireSlaveB.id, TwitterOnReceive);
    } else if (current == 2 && wireSlaveC.id != 0) {
        WireGetString(wireSlaveB.id, TwitterOnReceive);
    }
}

void TwitterWireMasterBegin() {
    WireBegin();
    TwitterWireMasterEnabled = 1;
}

void TwitterWireMasterAddSlave(int id, char *name) {
    if (wireSlaveA.id == 0) {
        wireSlaveA.id = id;
        strcpy(wireSlaveA.name, name);

    } else if (wireSlaveB.id == 0) {
        wireSlaveB.id = id;
        strcpy(wireSlaveB.name, name);

    } else if (wireSlaveC.id == 0) {
        wireSlaveC.id = id;
        strcpy(wireSlaveC.name, name);

    } else {
        Crash(3);
    }
}
//###############################################################

//###############################################################
//Twitter Serial
static int TwitterSerialEnabled = 0;

void TwitterSerialBegin() {
    TwitterSerialEnabled = 1;

    SerialBegin();

    OnSerial(TwitterOnReceive);
}
//###############################################################

//Global multicast to all interfaces
void TwitterSendToAll(char *message) {
    if (TwitterWireSlaveEnabled) {
    }

    if (TwitterWireMasterEnabled) {
        //Multicast to slaves
        if (wireSlaveA.id)
            WireSend(wireSlaveA.id, message, strlen(message)+1, NULL);
        if (wireSlaveB.id)
            WireSend(wireSlaveB.id, message, strlen(message)+1, NULL);
        if (wireSlaveC.id)
            WireSend(wireSlaveC.id, message, strlen(message)+1, NULL);
    }

    if (TwitterSerialEnabled) {
        SerialWrite(message);
    }
}

void TwitterSendToOne(char *name, char *message) {
    //Return the second we locate the person
    if (TwitterWireMasterEnabled) {
        if (wireSlaveA.id && !strcmp(wireSlaveA.name, name)) {
            WireSend(wireSlaveA.id, message, strlen(message)+1, NULL);
            return;
        } else if (wireSlaveB.id && !strcmp(wireSlaveB.name, name)) {
            WireSend(wireSlaveB.id, message, strlen(message)+1, NULL);
            return;
        } else if (wireSlaveC.id && !strcmp(wireSlaveC.name, name)) {
            WireSend(wireSlaveC.id, message, strlen(message)+1, NULL);
            return;
        }
    }

    if (TwitterWireSlaveEnabled) {
        strcpy(twitterWireSlaveMessageToSend, message);
    }

    //We can't know what's downstream here, just send a message
    if (TwitterSerialEnabled) {
        SerialWrite(message);
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
        if (hashtagA.callback && !strcmp(hashtagA.hashtag, tempBuffer)) {
            hashtagA.callback();
        } else if (hashtagB.callback && !strcmp(hashtagB.hashtag, tempBuffer)) {
            hashtagB.callback();
        } else if (hashtagC.callback && !strcmp(hashtagC.hashtag, tempBuffer)) {
            hashtagC.callback();
        }

        //Send message back if needed
        TwitterSendToAll(message);

    //Is this message directed towards me?
    } else if (!strcmp(tempBuffer, name)) {
        JsonGetString(message, "sub", tempBuffer);

        //Is this part of a session?
        if (tempBuffer[0] == 'c' && tempBuffer[1] == 'c') {
            int key = JsonGetValue(message, "key");
            //Find the callback
            if (sessionA.key == key) {
                Async(sessionA.callback, message);
                sessionA.key = -1;
            } else if (sessionB.key == key) {
                Async(sessionB.callback, message);
                sessionB.key = -1;
            }
        }

        //Is there any callbacks registered to this message
        else if (subjectA.callback && !strcmp(subjectA.subject, tempBuffer)) {
            subjectA.callback();
        } else if (subjectB.callback && !strcmp(subjectB.subject, tempBuffer)) {
            subjectB.callback();
        }
    }
}
//####################################################################

void TwitterSignUp(char *nameToUse) {
    strcpy(name, nameToUse);

    //Clear sessions
    sessionA.key = -1;
    sessionB.key = -1;

    //Clear hashtag
    hashtagA.callback = 0;
    hashtagB.callback = 0;
    hashtagC.callback = 0;

    //Init master slaves
    wireSlaveA.id = 0;
    wireSlaveB.id = 0;
    wireSlaveC.id = 0;

    //Clear subjects
    subjectA.callback = 0;
    subjectB.callback = 0;
}

void Tweet(char *to, char *subject, char *message, AsyncCallback_t callback) {
    //Used for tracking potentional 2-way callback sessions
    static unsigned char sessionKey = 0;

    JsonNew(tempBuffer);
    JsonSetString(tempBuffer, "to", to);
    JsonSetString(tempBuffer, "from", name);
    JsonSetString(tempBuffer, "sub", subject);
    JsonSetString(tempBuffer, "msg", message);

    //Add session
    if (callback) {
        static int nextSession = 0;

        if (nextSession == 0) {
            sessionA.callback = callback;
            sessionA.key = sessionKey;
        } else if (nextSession == 1) {
            sessionB.callback = callback;
            sessionB.key = sessionKey;
        }

        ++nextSession;
        nextSession = nextSession % 2;

        JsonSetValue(tempBuffer, "key", sessionKey);
    } else {
        JsonSetValue(tempBuffer, "key", -1);
    }

    //Hashtag (Multicast message)
    if (to[0] == '#')
        //Multicast message
        TwitterSendToAll(tempBuffer);
    else if (to[0] == '@')
        TwitterSendToOne(to, tempBuffer);

    ++sessionKey;
    sessionKey = sessionKey % 20;
}

void TweetValue(char *to, char *subject, int value, AsyncCallback_t callback) {
    static char message[10];
    sprintf(message, "%d", value);
    Tweet(to, subject, message, callback);
}


void TweetReturn(char *msg) {
    static int sessionKey = 0;
    sessionKey = JsonGetValue(AsyncMessage, "key");

    if (sessionKey == -1)
        return;

    static char to[20];
    static char from[20];

    JsonGetString(AsyncMessage, "to", to);
    JsonGetString(AsyncMessage, "from", from);

    JsonNew(tempBuffer);
    JsonSetValue(tempBuffer, "key", sessionKey);
    JsonSetString(tempBuffer, "to", from);
    JsonSetString(tempBuffer, "from", to);
    JsonSetString(tempBuffer, "subject", "cc");
    JsonSetString(tempBuffer, "msg", msg);

    TwitterSendToOne(from, tempBuffer);
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

void TwitterRegisterSubject(char *subject, AsyncCallback_t callback) {
      if (!subjectA.callback) {
        subjectA.callback = callback;
        strcpy2(subjectA.subject, subject);
    } else if (!subjectB.callback) {
        subjectB.callback = callback;
        strcpy2(subjectB.subject, subject);
    } else {
        Crash(3);
    }
}