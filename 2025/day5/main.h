#pragma once
#include <stdio.h>
#include <stdint.h>

typedef struct {
   uint64_t start;
   uint64_t end;
}range;

int pt1(FILE* fp);
int pt2(FILE* fp);
