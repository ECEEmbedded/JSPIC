#ifndef TWITTER_H_
#define TWITTER_H_

#include "async.h"
#include "json.h"
#include "crashHelper.h"
#include "jwire.h"
#include "wireMaster.h"

//Register as a twitter user
void TwitterSignUp(char *name);

//Send a message
void Tweet(char *subject, char *to, char *message, AsyncCallback_t callback);
void TweetReturn(char *msg);  //Return a message from a callback

//What to do when we receive a certain hashtag
void TwitterRegisterHashtag(char *hashtag, AsyncCallback_t callback);

//Call this when receiving a mesasge from some interface
void TwitterOnReceive();

//##################
//Twitter slave wire
void TwitterWireSlaveBegin(int id);
//##################

//##################
//Twitter master wire
void TwitterWireMasterBegin();
//##################

#endif