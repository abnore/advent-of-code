#include "main.h"
#include <blackbox.h>

int pt1test(FILE* fp){
    (void)fp;

    int dx, dy;
    int x = 0;
    int y = 100;

    int highest = 0;

    while(x <= 100)
    {
        int capacity = (2*y - x);
        int durability = (3*y - 2*x);
        int flavor = (6*x - 2*y);
        int texture = (3*x - y);

        if(capacity < 0) capacity = 0;
        if(durability < 0) durability= 0;
        if(flavor < 0) flavor= 0;
        if(texture < 0) texture= 0;

        int res = capacity * durability * flavor * texture;

        if (res > highest){
            dx = x;
            dy = y;
            highest = res;
        }
        x++;
        y--;
    }

    INFO("x: %i, y: %i", dx, dy);

    return highest;
}
