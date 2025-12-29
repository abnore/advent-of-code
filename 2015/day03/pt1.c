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

int pt1(FILE* fp){

    Vec2 start = {0,0};
    Vec2 cache[10000]={0};
    int seen=1; // including the house we start at

    char c;
    while ( (c=fgetc(fp)) != '\n' )
    {
        switch (c){
            case '^': start.y+=1; break;
            case '>': start.x+=1; break;
            case '<': start.x-=1; break;
            case 'v': start.y-=1; break;
        }
        if(visited(start,cache, seen)){
            continue; // dont cache it, and dont count it again
        }
        cache[seen++] = start; // now cache it for later
    }
    return seen;
}
