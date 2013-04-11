#include "twitter.h"
#include "jwire.h"

//Use this for string manipulations, just free space...
static char tempBuffer[200];

//Used to maintain 2-way sessions (Callbacks)
typedef struct SESSION {
    int key;
    AsyncCallback_t callback;
} Session;
Session sessionA;
Session sessionB;
Session sessionC;
Session sessionD;
#define SessionAVacant (sessionA.key == -1)
#define SessionBVacant (sessionB.key == -1)
#define SessionCVacant (sessionB.key == -1)
#define SessionDVacant (sessionB.key == -1)

//Modify these for your platforms specific needs
//####################################################################

//Global multicast to all interfaces
void TwitterSendToAll(char *message) {
}

//Register this callback function for incomming messages
void TwitterOnReceive() {
    char *message = AsyncMessage;
    
    //Get the 'to' field of this message
    JsonGetString(message, "to", tempBuffer);

    //Hashtag multicast
    if (tempBuffer[0] == '#') {
        //Is there any callbacks registered to this message?

        //Send message back if needed

        TwitterSendToAll(message);
    }
}

//Setup interfaces, etc.
void TwitterCustomSetup() {
}

//Add Twitter to I2C Slave
void TwitterAddWireSlave(int id) {
    JWireBegin(id);
    JWireOnReceive(TwitterOnReceive);
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
}

void Tweet(char *subject, char *to, char *message, AsyncCallback_t callback) {
    //Used for tracking potentional 2-way callback sessions
    static int sessionKey = 0;
    
    //Hashtag (Multicast message)
    if (to[0] == '#') {
        JsonNew(tempBuffer);
        JsonSetString(tempBuffer, "from", name);
        JsonSetString(tempBuffer, "msg", message);
        JsonSetString(tempBuffer, "to", to);

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
}