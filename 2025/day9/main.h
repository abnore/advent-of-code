#pragma once
#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint64_t x, y;
}Vec2;

int pt1(FILE* fp);
int pt2(FILE* fp);
