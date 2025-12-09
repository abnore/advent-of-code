#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

typedef struct {
    int x1, y1;
    int x2, y2;
} Segment;

/* Returns a 1 if a segment (made of red-red tiles) cuts through the interior
 * of a candidate rectangle.
 *
 * If so, the rectangle is INVALID, and we break out later - because its
 * insides contains non-green tiles.
 */
static inline int segment_intersects_rect(Segment s, int min_x, int max_x, int min_y, int max_y)
{
    int x1 = s.x1, y1 = s.y1;
    int x2 = s.x2, y2 = s.y2;

    if (x1 == x2) {
        /* Vertical segment starting from x = x1 */
        int x = x1;
        if (x > min_x && x < max_x) {
            /* Check if it vertically passes through inside */
            int sy = (y1 < y2) ? y1 : y2;
            int by = (y1 > y2) ? y1 : y2;
            if (by > min_y && sy < max_y)
                return 1; // invalid
        }
    } else {
        /* Horizontal segment starting from y = y1 */
        int y = y1;
        if (y > min_y && y < max_y) {
            int sx = (x1 < x2) ? x1 : x2;
            int bx = (x1 > x2) ? x1 : x2;
            if (bx > min_x && sx < max_x)
                return 1; // invalid
        }
    }

    /* All good, the boundries didnt intersect with a non red or green tile */
    return 0;
}

int pt2_no_raster(FILE* fp){
    
    /* Find the length of the file */
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    rewind(fp);
    /* prepare for getline and read line by line after malloc */
    Vec2 *tiles = malloc(len*sizeof(Vec2)); // Overkill, but reallocated after

    char *line = NULL;
    size_t cap = 0;
    int num_tiles=0;

    /* While reading in, storing the biggest x and y as width and height */
    int width=0, height=0;
    while(getline(&line,&cap,fp) > 0){
        sscanf(line,"%llu,%llu",&tiles[num_tiles].x, &tiles[num_tiles].y);
        if(tiles[num_tiles].x > width) width = tiles[num_tiles].x ;
        if(tiles[num_tiles].y > height) height = tiles[num_tiles].y ;
       
        num_tiles++;
    }
    tiles = realloc(tiles, num_tiles*sizeof(Vec2));

    /* Since rastering took over 10G of RAM on the real input, and gave me a bus error, i need
     * to take the same approach only pure computationally */
    Segment *segments = malloc(num_tiles * sizeof(Segment));
    int seg_count = 0;

    /* Build boundary segments consisting of the red tile-pairs instead of drawing on a grid */
    for (int i = 0; i < num_tiles; i++) {
        int cur_x  = tiles[i].x;
        int cur_y  = tiles[i].y;
        int next_x = tiles[(i+1==num_tiles)?0:i+1].x;
        int next_y = tiles[(i+1==num_tiles)?0:i+1].y;

        /* Horizontal or vertical */
        segments[seg_count++] = (Segment){cur_x, cur_y, next_x, next_y};
    }

    /* Read every tile in, now we calculate all the areas and print the biggest */
    uint64_t biggest_area=0;
    for(int i=0;i<num_tiles-1;++i)
    {
        int64_t x_1 = (int64_t)tiles[i].x;
        int64_t y_1 = (int64_t)tiles[i].y;
        for(int ii=i+1;ii<num_tiles;++ii)
        {
            int64_t x_2 = tiles[ii].x;
            int64_t y_2 = tiles[ii].y;
            if(x_1 == x_2) continue; // No points in doing the lines.. **MAYBE BUG** 
            if(y_1 == y_2) continue; // No points in doing the lines.. **MAYBE BUG** 
            
            uint64_t distx=llabs((x_2-x_1));
            uint64_t disty=llabs((y_2-y_1));
            uint64_t area = (distx+1) * (disty+1);    

            /* Here i need to check something... And that something is:
             * Normalizing the coordinates, so we can pass from top left
             * to bottom right. Then check the intersections */
            int inner_x;
            int inner_y;

            if(x_1<x_2){
                inner_x=x_1;
            } else {
                inner_x=x_2;
            }
            if(y_1<y_2){
                inner_y=y_1;
            } else {
                inner_y=y_2;
            }

            /* As we did in pt1 we computed the area pure, here we do the same, but then 
             * also check the intersection of rectangles and possible segments */
            if (area > biggest_area){
                int valid = 1;

                /* check this against every segment for intersection */
                for (int k = 0; k < seg_count; k++){
                    if (segment_intersects_rect(
                                segments[k], 
                                inner_x, inner_x + distx, 
                                inner_y, inner_y + disty)){
                        valid = 0;
                        break;
                    }
                }

                if (valid)
                    biggest_area = area;
            }
        }
    }

    INFO("Biggest area is %llu",biggest_area);

    free(tiles);
    free(line);
    free(segments);
    return 0;
}
