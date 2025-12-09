#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

const char* colors[] = {
    "\033[0;31m", // Red
    "\033[0;32m", // Green
    "\033[0;90m"  // Gray
};
const char* c_reset = "\033[0m";

void print_grid(int width, int height, char* grid){
    int c;
    char symbol;

    printf("%s", c_reset);
    for(int col = 0; col<height;++col){
        for(int row = 0; row<width; ++row){
            symbol = grid[col*width+row];
            if(symbol=='X')c=GREEN;
            if(symbol=='.')c=GRAY;
            if(symbol=='#')c=RED;
            printf("%s%c%s", colors[c], symbol, c_reset);
        }
        printf("\n");
    }
}

int comp_tiles(const void *a, const void *b)
{
    int ret = 0;
    /* Here we need logic to sort the tiles */
    Vec2 *va = (Vec2*)a;
    Vec2 *vb = (Vec2*)b;

    TRACE("Comparing %llu,%llu and %llu,%llu", va->x,va->y, vb->x,vb->y );
    /* Sort by column and then by row - so y then x */
    if(va->y < vb->y)       ret = -1;
    else if(va->y > vb->y)  ret = 1;
    if(va->y == vb->y){
        if(va->x < vb->x)   ret = -1;
        if(va->x == vb->x)  ret = 0;
        if(va->x > vb->x)   ret = 1;
    }
    return ret;
}

/* ======== */

int pt2(FILE* fp){
    
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

    /* Allocating for the grid and for sorting so i can draw easy */
    char *grid = malloc(len*sizeof(char));
    Vec2 *draw_tiles = malloc(num_tiles*sizeof(Vec2));
    memcpy(draw_tiles, tiles, num_tiles*sizeof(Vec2));

    /* Sort the coordinates for drawing and searching */
    DEBUG("Sorting tiles");
    qsort(draw_tiles, (size_t)num_tiles, sizeof(Vec2), comp_tiles);

    /* I will construct the grid and print it */
    width+= 3; height+=2;
    DEBUG("Biggest x is %i, y is %i after +3 to width, +2 to height", width, height);

    int64_t x_1, y_1;
    int64_t x_2, y_2;

    for(int col=0; col<height; ++col) {
        char c;
        for(int row=0; row<width; ++row) {
            int rel_pos = col*width+row;
            grid[rel_pos] = '.';
        }
    }

    for(int i=0; i<num_tiles;++i)
    {
        Vec2 t = draw_tiles[i];
        int row = (int)t.x;
        int col = (int)t.y;
        int rel_pos = col*width+row;
        INFO("tile[%i] x:%llu y:%llu rel_pos: %i", i, t.x, t.y, rel_pos  );

        grid[rel_pos] = '#';
    }

    /* Now i have the grid correctly, and i can print to verify
     * Filling in green by first playing some snake. 
     
     * We have to take the largest square and subtract all squares, NOT
     * inside that overlap - if that makes sense. The largest is still largest,
     * but now we have to take away from it. A_1 - A_2 = A_tot
     */


    int safety=30;
    int tiles_idx=0;
    int rel_pos;

    /* Looping over the non sorted list, which is in order */
    for(int i=0; i<num_tiles; i++){
        int cur_x = tiles[i].x; 
        int cur_y = tiles[i].y; 
        int next_x=tiles[(i+1)==num_tiles?0:i+1].x;
        int next_y=tiles[(i+1)==num_tiles?0:i+1].y;

        /* And when having a difference, fill in between them */
        INFO("cur x:%i y:%i, next x:%i y:%i", cur_x, cur_y, next_x, next_y);
        while(cur_x!=next_x)
        {
            if(cur_x < next_x)
            {
                cur_x+=1;
            } else {
                cur_x-=1;
            }
            rel_pos = cur_y*width + cur_x;
            if(grid[rel_pos] != '#')
                grid[rel_pos] = 'X'; 
        }
        while(cur_y!=next_y)
        {
            if(cur_y < next_y)
            {
                cur_y+=1;
            } else {
                cur_y-=1;
            }
            rel_pos = cur_y*width + cur_x;
            if(grid[rel_pos] != '#')
                grid[rel_pos] = 'X'; 
        }
    }

    print_grid(width,height,grid);

    /* Now we loop over the entire grid, and fill out the rest */
    int inside=0;
    for(int col=0; col<height; col++){
        for(int row=0; row<width; row++){
            int rel_pos = col*width+row;
            int rel_edge = col*width+(width-1);
            char c = grid[rel_pos];
            char cn = grid[rel_pos==rel_edge?rel_edge:rel_pos+1];

            /* There are 3 cases - either a #X, #. X. - that all are inside
             * What determines that, is the rest of the row. */

            if(c=='#'){ // If the next is X we are on a wall
                if(cn=='X'){
                    continue;
                } else if(cn=='.'){ // check the rest of the row, only way 
                    for(int i=rel_pos; i<rel_edge;++i) {
                        if(grid[i]=='X'){ // At ANY point we are inside!
                            inside=1;
                            break;
                        }
                    }
                    // If we reach here and didnt break we are outside
                    continue;
                }
            }
            if(c=='X'){ // Either X. , X# or XX
                if(cn=='X'){ // We are on the wall, continue next
                    continue;
                } else if(cn=='.'){ // This can happend two times, so we toggle!
                    inside=!inside;
                    continue;
                }
            }
            /* If we make it here we draw in based on if we are inside or not! */
           if(inside) grid[rel_pos]='X'; 
        }
    }

    print_grid(width,height,grid);



    free(tiles);
    free(draw_tiles);
    free(line);
    free(grid);
    return 0;
}
