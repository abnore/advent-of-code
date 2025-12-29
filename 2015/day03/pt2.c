#include "main.h"
#include <blackbox.h>

typedef struct {
    int x, y;
} Vec2;

static int visited(Vec2 coord, Vec2 *cache, int cache_size){
   for(int i=0; i<cache_size; ++i)
       if (coord.x == cache[i].x && coord.y == cache[i].y)
            return 1; // Been here before
   return 0; // havent been
}

static void move(Vec2 *coord, char dir)
{
    switch (dir){
        case '^': coord->y+=1; break;
        case '>': coord->x+=1; break;
        case '<': coord->x-=1; break;
        case 'v': coord->y-=1; break;
    }

}
int pt2(FILE* fp){

    Vec2 santa = {0,0};
    Vec2 robot = {0,0};
    int flip=0;
    Vec2 cache[10000]={0};

    int seen=1; // including the house we start at

    char c;
    while ( (c=fgetc(fp)) != '\n' )
    {
        flip ^= 1;

        if(flip) {
            move(&santa, c);
            if(visited(santa, cache, seen))
                continue; // dont cache it, and dont count it again
            cache[seen] = santa; // now cache it for later
        } else {
            move(&robot, c);
            if(visited(robot, cache, seen))
                continue; // dont cache it, and dont count it again
            cache[seen] = robot; // now cache it for later
        }
        seen++;
    }
    return seen;
}
