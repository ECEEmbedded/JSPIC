#ifndef TWITTER_H_
#define TWITTER_H_

#include "async.h"
#include "json.h"
#include "crashHelper.h"
#include "jwire.h"
#include "wireMaster.h"
#include "serial.h"
#include "interval.h"
#include "flipper.h"

//Register as a twitter user
void TwitterSignUp(char *name);

//Send a message
void Tweet(char *to, char *subject, char *message, AsyncCallback_t callback);
void TweetValue(char *to, char *subject, int message, AsyncCallback_t callback);
void TweetReturn(char *msg);  //Return a message from a callback
void TweetReturnValue(int value);   //Yeahhhhhh

//What to do when we receive a certain hashtag
void TwitterRegisterHashtag(char *hashtag, AsyncCallback_t callback);

//What to do when we receive a certain subject
void TwitterRegisterSubject(char *subject, AsyncCallback_t callback);

//Get data from message
int TweetGetValue();
void TweetGetString(char *output);

//Call this when receiving a mesasge from some interface
void TwitterOnReceive();

//Proxy to TwitterOnReceive to avoid loopbacks in multicasts (Don't send back)
void TwitterOnReceiveFromSerial();
void TwitterOnReceiveFromWireMaster();
void TwitterOnReceiveFromWireSlave();

//##################
//Twitter slave wire
void TwitterWireSlaveBegin(int id);
//##################

//##################
//Twitter master wire
void TwitterWireMasterBegin();
void TwitterWireMasterAddSlave(int id, char *name);
//##################

//##################
//Twitter Serial
void TwitterSerialBegin();
//##################

#endif