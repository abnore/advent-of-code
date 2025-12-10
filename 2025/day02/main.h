#pragma once
#include <stdio.h>

typedef struct {
    int64_t start;
    int64_t end;
} range;

int pt1(FILE* fp);
int pt2(FILE* fp);
