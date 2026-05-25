#include "main.h"
#include <blackbox.h>
#include <string.h>

#define WIDTH 100
#define HEIGHT 100
// #define WIDTH 6
// #define HEIGHT 6
#define SIZE WIDTH*HEIGHT


static void print_grid(char *grid){
    for(int j=0; j<HEIGHT; j++){
        for(int i=0; i<WIDTH; i++){
            printf("%c",grid[j*HEIGHT + i]);
        }
        printf("\n");
    }
}

/*
z = y*height + x;
x = z % WIDTH;
y = z / HEIGHT;

*/

static int count_n(char *grid, int x, int y){
    int count=0;
    for(int j=-1; j<2; j++){
        int dy = y+j;
        if (dy < 0 || dy > HEIGHT-1) continue;
        for(int i=-1; i<2; i++){
            int dx = x+i;
            if (dx < 0 || dx > HEIGHT-1 || (j==0 && i==-0)) continue;
            if (grid[dy * HEIGHT + dx] != '.') count++;
        }
    }
    return count;
}

static int count_lights(char *grid){
    int count=0;
    for(int i=0; i<SIZE; i++){
        if (grid[i] != '.') count++;
    }
    return count;
}
/* The state a light should have next is based on its current state (on or off)
   plus the number of neighbors that are on:

   A light which is on stays on when 2 or 3 neighbors are on, and turns off
   otherwise.
   A light which is off turns on if exactly 3 neighbors are on, and
   stays off otherwise.
   All of the lights update simultaneously; they all
   consider the same current state before moving to the next.*/
static void update(char *grid)
{
    char temp_grid[SIZE];

    for(int i=0; i<SIZE; i++){
        char current = grid[i];

        int neighbours = count_n(grid, i%WIDTH, i/HEIGHT);

        if (neighbours == 3) {
            temp_grid[i] = (char)(neighbours + 48);
            continue;
        }
        if (neighbours == 2) {
            if (current != '.'){
                temp_grid[i] = (char)(neighbours + 48);
                continue;
            }
        }
        temp_grid[i] = '.';
    }


    // 4 corners always '#' 0,0 / WIDTH-1, 0 / 0, HEIGHT-1/ WIDTH-1, HEIGHT-1
    temp_grid[0] = '#';
    temp_grid[WIDTH-1] = '#';
    temp_grid[SIZE-1] = '#';
    temp_grid[SIZE-WIDTH] = '#';

    memcpy(grid, temp_grid, SIZE);
}


int pt2(FILE* fp)
{
    char grid[SIZE];
    int ch, idx=0, answer;
    while( (ch=(char)fgetc(fp)) != EOF) if(ch != '\n') grid[idx++] = ch;

    int generations = 100;

    // activate corners
    grid[0] = '#';
    grid[WIDTH-1] = '#';
    grid[SIZE-1] = '#';
    grid[SIZE-WIDTH] = '#';

    for(int i=0; i<generations; i++){
        update(grid);
        answer=count_lights(grid);
    }

    return answer;
}

