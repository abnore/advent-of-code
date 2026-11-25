#include "main.h"
#include <blackbox.h>
#include <string.h>
#include <stdint.h>

#define ROW 2947
#define COL 3029

struct coord{
    int row;
    int col;
    uint64_t num;
    int range;
};

// helper for counting. starts at 1,1, then 2,1 - 2,2, then 3,1 etc,
// where the first number is row, second is col
struct coord next_col_row(struct coord c) {
    c.row--;
    c.col++;

    if(c.row < 1) {
        c.range++;
        c.row = c.range;
        c.col = 1;
    }
    return c;
}

struct coord calc_next(struct coord c) {
    struct coord ret = next_col_row(c);
    uint64_t a = c.num * 252533;
    ret.num = a % 33554393;

    return ret;
}

uint64_t pt1(void)
{

    struct coord c = { .col = 1, .row = 1, .num = 20151125, .range = 1 };
    // for(int i = 0; i < 15; i++){
    while(1) {
        if (c.col==COL && c.row==ROW) break;
        c = calc_next(c);
    }
    printf(" (%i,%i) - %llu\n", c.row, c.col, c.num);

    return 0;
}
