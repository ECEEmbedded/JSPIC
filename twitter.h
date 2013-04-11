#ifndef TWITTER_H_
#define TWITTER_H_

#include "async.h"
#include "json.h"

//Register as a twitter user
void TwitterSignUp(char *name);

//Send a message
void Tweet(char *subject, char *to, char *message, AsyncCallback_t callback);
void TweetReturn(char *msg);  //Return a message from a callback

//What to do when we receive a certain hashtag
void TwitterRegisterHashtag(char *hashtag, AsyncCallback_t callback);

void TwitterAddWireSlave(int id);

#endif