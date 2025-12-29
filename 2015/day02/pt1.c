#include "main.h"
#include <blackbox.h>

#define _min(a,b) (a<b?a:b)
#define min(a,b,c) _min(a,_min(b,c))

int pt1(FILE* fp){
    int l,w,h;
    char *line = NULL;  // getline allocates when line==NULL
    size_t cap = 0;     // capacity for line (how much allocated)
   
    int total_area = 0;
    while(getline(&line, &cap, fp)>0){
        sscanf(line, "%ix%ix%i\n", &l, &w, &h);

        INFO("line is %ix%ix%i", l, w, h);
        int lw = l*w;
        int wh = w*h;
        int hl = h*l;
        int area = 2*(lw+wh+hl) + min(lw,hl,wh);

        INFO("area is %i", area);

        total_area += area;
    }

    INFO("total area: %i", total_area);
    return 0;
}
