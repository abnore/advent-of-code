#pragma once
#include <stdio.h>
#include <stdint.h>

typedef struct Jbox Jbox;
struct Jbox{
    uint64_t x, y, z;
};

typedef struct{
    uint64_t dist;
    Jbox *a;
    Jbox *b;
    int ia; // indexes into boxes[i]
    int ib; // the only way i could think of to access them later in O(1)
}Pair;

int pt1(FILE* fp);
int pt2(FILE* fp);
