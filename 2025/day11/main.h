#pragma once
#include <stdio.h>

#define da_append(xs, x) \
     do {\
        if(xs.count >= xs.capacity){\
            if (xs.capacity == 0) xs.capacity = 256;\
            xs.capacity <<= 1;\
            xs.items = realloc(xs.items, xs.capacity*sizeof(*xs.items));\
        }\
        xs.items[xs.count++] = x;\
    }while(0)\

#define da_free(xs) \
    do { \
        free((xs).items); \
        (xs).items = NULL; \
        (xs).count = (xs).capacity = 0; \
    } while (0)
 
int pt1(FILE* fp);
int pt2(FILE* fp);
