#pragma once
#include <stdio.h>

#define strip(str) do{\
        char *__s=str;\
        int __l=0; while(*__s) {__l++; *__s++;}\
        if(str[__l-1]=='\n') str[__l-1]=0;\
        if(str[__l-2]=='.')  str[__l-2]=0;\
        }while(0)

typedef struct{
    char name[16];
    int speed;
    int duration;
    int rest;
/* part 2 extras */
    int distance;
    int points;
    int moving;
    int resting;
    int countdown;
}reindeer;

int pt1(FILE* fp);
int pt2(FILE* fp);
