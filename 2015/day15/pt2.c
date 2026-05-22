#include "main.h"
#include <blackbox.h>

#define teaspoons 100

int pt2(FILE* fp){
    (void)fp;

    int ia, ib, ic, id;
    int highest = 0;

    for(int a=0; a<=teaspoons; a++){
        for(int b=0; b<=teaspoons-a; b++){
            for(int c=0; c<=teaspoons-b-a; c++ ){
                int d= teaspoons - a - b - c;

                int capacity    = 2*a;
                int durability  = 5*b - d;
                int flavor      = 5*c - capacity - 3*b;
                int texture     = 5*d - c;
                int calories    = 3*a + 3*b + 8*c + 8*d;

                if(capacity < 0) capacity = 0;
                if(durability < 0) durability= 0;
                if(flavor < 0) flavor= 0;
                if(texture < 0) texture= 0;

                int res = capacity * durability * flavor * texture;

                if (res > highest && calories == 500){
                    ia = a;
                    ib = b;
                    ic = c;
                    id = d;
                    highest = res;
                }

            }
        }
    }

    INFO("a: %i, b: %i, c: %i, d: %i", ia, ib, ic, id);

    return highest;
}

