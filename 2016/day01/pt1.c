#include "main.h"
#include <blackbox.h>

// R increases the number, L decreases, and it wraps
// 2 bits, 11, which contains all 4 directions, and naturally wraps
enum {
    NORTH,  // 0  +y
    EAST,   // 1  +x
    SOUTH,  // 2  -y
    WEST    // 3  -x
};

#define turn_right(a) ({(a+=1); a&=3;})
#define turn_left(a) ({(a-=1); a&=3;})

struct coord {
    int x;
    int y;
    int dir;
};
static inline void add_dir(struct coord *c, int blocks){
    switch(c->dir) {
        case NORTH: c->y += blocks; return;
        case EAST:  c->x += blocks; return;
        case SOUTH: c->y -= blocks; return;
        case WEST:  c->x -= blocks; return;
    }
}

void walk(char *buf, struct coord *start)
{
    int idx=0;
    int c, num=0;
    char *num_str;
    do {
        c = buf[idx++];
        // printf("%c", c);
        switch(c){
            case 'R':
                turn_right(start->dir);
                num_str= &buf[idx++];
                num = atoi(num_str);
                // printf(" %d ", num);
                add_dir(start, num);
                break;
            case 'L':
                turn_left(start->dir);
                num_str = &buf[idx++];
                num = atoi(num_str);
                // printf(" %d ", num);
                add_dir(start, num);
                break;
        }
        while(buf[idx++] != ' '); // in case longer nums, skip to space
    }
    while(c);
}

int pt1(FILE* fp) {
    char bigbuf[500000];
    size_t pos = 0;

    int c;
    while ((c = fgetc(fp)) != EOF) {
        if ( c==',') continue;
        bigbuf[pos++] = (char)c;
    }
    bigbuf[pos] = '\0';

    struct coord start = {0};
    walk(bigbuf, &start);

    return (start.x + start.y);
}
