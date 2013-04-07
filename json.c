//
//  json.c
//  learningStrings
//
//  Created by Matthew Ibarra on 4/6/13.
//  Copyright (c) 2013 Matthew Ibarra. All rights reserved.
//

#include <stdio.h>


#include "json.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COLON_SPACE ": "

#define MAX_STRING_SIZE 200

static char KEY_COLON_SPACE[MAX_STRING_SIZE];  // Shouldn't need more than 200
static char result[MAX_STRING_SIZE];   // Shouldn't need more than 200

void jsonNew(char* string) {
    sprintf(string, "{\n}");
}

int jsonGetValue(char* input, char* key) {
    static char KEY_COLON_SPACE[MAX_STRING_SIZE];  // Shouldn't need more than 200
    // Will know to look here if this becomes a problem.
    sprintf(KEY_COLON_SPACE, "\"%s\"%s", key, COLON_SPACE);
    char* substr_s = strstr(input, KEY_COLON_SPACE);

    substr_s = strstr(substr_s, " ");
    substr_s++;     //Removes the first character in the char array

    if(substr_s != NULL) {
        char* substr2_s = strstr(substr_s, ",\n");
        unsigned long length_substr_s = strlen(substr_s);
        unsigned long length_substr2_s = strlen(substr2_s);
        unsigned long length_sub_s = length_substr_s - length_substr2_s;
        // Will know to look here if this becomes a problem.
        for (int i=0; i<length_sub_s; i++) {
            result[i] = substr_s[i];
        }

        result[length_sub_s] = 0;

        int result_float = 0;

        if(result[0] == '\"') {     // If it's quoted, remove the quotes.
            char *p = result;
            p++;
            p[strlen(p)-1] = 0;
            result_float = atoi(p);
        }
        else {
            result_float = atoi(result);
        }
        return result_float;
    }
    return -696969;
}

void jsonSetString(char* input, char* key, char* str) {
    unsigned long original_json_input_len = strlen(input);
    input[original_json_input_len - 1] = 0;
    strcpy2(KEY_COLON_SPACE, input);
    sprintf(input, "%s\"%s\": \"%s\",\n}", KEY_COLON_SPACE, key, str);
}

void jsonSetValue(char* input, char* key, int value) {

    unsigned long original_json_input_len = strlen(input);
    input[original_json_input_len - 1] = 0;
    strcpy2(KEY_COLON_SPACE, input);
    sprintf(input, "%s\"%s\": %d,\n}", KEY_COLON_SPACE, key, value);
}

int jsonGetString(char* input, char* key, char* string) {
    // Will know to look here if this becomes a problem.
    sprintf(KEY_COLON_SPACE, "\"%s\"%s", key, COLON_SPACE);
    char* substr_s = strstr(input, KEY_COLON_SPACE);

    substr_s = strstr(substr_s, " ");
    substr_s++;     //Removes the first character in the char array

    if(substr_s != NULL) {
        char* substr2_s = strstr(substr_s, ",\n");
        unsigned long length_substr_s = strlen(substr_s);
        unsigned long length_substr2_s = strlen(substr2_s);
        unsigned long length_sub_s = length_substr_s - length_substr2_s;
        // Will know to look here if this becomes a problem.
        for (int i=0; i<length_sub_s; i++) {
            result[i] = substr_s[i];
        }

        result[length_sub_s] = 0;

        char *p = result;
        p++;
        p[strlen(p)-1] = 0;
        strcpy2(string, p);
        return 0;
    }
    return -1;
}