#include "main.h"
#include <blackbox.h>
#include <ctype.h>

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

typedef struct {
    int x,y;
} Vec2;

#define WIDTH 1024
static int direction=0; // 4 directions
static Vec2 start_g = {.x=WIDTH/2, .y=WIDTH/2};
static int map_g[WIDTH*WIDTH]={0};

static inline int create_num(char *buf, int *idx){
    int i=*idx;
    int num = atoi(&buf[i]);
    while (isdigit(buf[i])) i++;
    *idx = i;
    return num;
}

static inline int coord_to_idx(Vec2 s){
    return (s.y * WIDTH + s.x);
}

static inline Vec2 idx_to_coord(int idx){
    int x = idx % WIDTH;
    int y = idx / WIDTH;
    return ((Vec2){.x=x, .y=y});
}

static void walk( char* buf){
    int idx=0;
    char c;
    int num;
    do {
        c=buf[idx++];
        num=create_num(buf, &idx);
        if(num==0) continue;

        switch(c){
        case 'R': turn_right(direction); break;
        case 'L': turn_left(direction); break;
        }

        int start_pos = coord_to_idx(start_g);

        for(int i=0; i<num; i++){

            if(map_g[start_pos] == 1) {
                start_g = idx_to_coord(start_pos);
                return;
            }

            switch(direction){
                case NORTH:
                    map_g[start_pos] = 1;
                    start_pos -= WIDTH;
                    start_g.y -= 1;
                    break;
                case EAST:
                    map_g[start_pos] = 1;
                    start_pos += 1;
                    start_g.x += 1;
                    break;
                case SOUTH:
                    map_g[start_pos] = 1;
                    start_pos += WIDTH;
                    start_g.y += 1;
                    break;
                case WEST:
                    map_g[start_pos] = 1;
                    start_pos -= 1;
                    start_g.x -= 1;
                    break;
            }

        }
    }
    while(c);

    return;
}

int pt2(FILE* fp){
    char bigbuf[500000];
    size_t pos = 0;

    int c;
    while ((c = fgetc(fp)) != EOF) {
        bigbuf[pos++] = (char)c;
    }
    bigbuf[pos] = '\0';

    walk( bigbuf );

    // printf("x: %d\n", start_g.x);
    // printf("y: %d\n", start_g.y);

    int x = start_g.x - WIDTH/2;
    int y = start_g.y - WIDTH/2;

    // We have to take abs value, since negative respons is still moved
    x = (x<0) ? -x : x;
    y = (y<0) ? -y : y;

    // printf("x: %d\n",x);
    // printf("y: %d\n",y);

    return ( x + y);
}
