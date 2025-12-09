#pragma once
#include <stdio.h>
#include <stdint.h>

/* Borrowing this from my own library - blackbox for printing the grid */
typedef enum {
   RED,
   GREEN,
   GRAY
} COLORS;

typedef struct {
    uint64_t x, y;
}Vec2;

int pt1(FILE* fp);
int pt2(FILE* fp);
