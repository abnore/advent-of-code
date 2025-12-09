#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

int pt1(FILE* fp){
    
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    rewind(fp);

    char *line = NULL;
    size_t cap = 0;
    Vec2 *tiles = malloc(len*sizeof(int)); // this is overkill but fuck it
    int num_tiles=0;

    while(getline(&line,&cap,fp) > 0){
        sscanf(line,"%llu,%llu",&tiles[num_tiles].x, &tiles[num_tiles].y);
        DEBUG("Read a line with %d,%d", tiles[num_tiles].x, tiles[num_tiles].y);
        num_tiles++;
    }

    INFO("Read in %d tiles", num_tiles);
    uint64_t biggest_area=0;

    /* Read every tile in, now we calculate all the areas and print the biggest */
    INFO("permutations of the tiles are"); 
    for(int i=0;i<num_tiles-1;++i)
    {
        int64_t x_1 = (int64_t)tiles[i].x;
        int64_t y_1 = (int64_t)tiles[i].y;
        for(int ii=i+1;ii<num_tiles;++ii)
        {
            int64_t x_2 = tiles[ii].x;
            int64_t y_2 = tiles[ii].y;
           
            TRACE("x_1=%i x_2=%i y_1=%i y_2=%i",x_1, x_2, y_1,y_2);
            // (x_1-x_2+1)*(y_1-y_2+1);

            uint64_t distx=llabs((x_2-x_1));
            uint64_t disty=llabs((y_2-y_1));
            uint64_t area = (distx+1) * (disty+1);    

            if(area > biggest_area) biggest_area = area;
            DEBUG("%lli,%lli-%lli,%lli area = %llu", x_1,y_1,x_2,y_2, area);

        }
    }

    INFO("biggest area is %llu", biggest_area);
    free(tiles);
    free(line);
    return 0;
}
