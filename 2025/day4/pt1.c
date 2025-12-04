#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int count_neighbours(char* buf, int w, int h, int pos )
{
    // the grid based on the coord given is:
    // top      :  side         :  bottom
    // y-1,x-1    y-1,x             y-1,x+1
    //   y,x-1      y,x (SKIP)        y,x+1
    // y+1,x-1    y+1,x             y+1,x+1
    //char me = buf[pos];
    int neighbours=0;
    // Loop through all possible neighbours and check value. If . no, if @ +1
    int rel_x = pos % w;
    int rel_y = pos / h;
    for(int y=-1; y<=1; y++){
        int new_col = rel_y + y;
        if (new_col < 0 || new_col >= h ) continue;
        for(int x=-1; x<=1; x++){
            if(x==0 && y==0) continue; // This is me, dont count
            int new_row = rel_x + x;
            if(new_row < 0 || new_row >= w) continue;

            int n_pos = new_col*w+new_row;
            if(buf[n_pos] == '@') {
                neighbours++;
                DEBUG("pos %i nb %i is (%i-%i) with pos %i",pos, neighbours, new_col, new_row, n_pos);
            }

        }
    }
    return neighbours;
}
// Game of life logic, counting the square surrounding it
// Will not work on edge cases, need boundry checking
// And row col logic
static int count_rolls(char* buf, int w, int h)
{
    char p;
    DEBUG("width=%i height=%i", w, h);
   // (col*h+row) will take you to any coord where i and j will be looped over
    int total = w*h;
    int rolls = 0;
    int num_adjacent=0;
    for(int i=0; i <total; ++i)
    {
        if(buf[i] =='@'){
            num_adjacent = count_neighbours(buf, w, h, i);
            if(num_adjacent < 4) rolls++;
        }
    }
    return rolls;
}


int pt1(FILE* fp)
{
    char buf[32000]={0};
    int rolls=0;
    int ch, i=0, w=0, h=0, nl=0;
    while((ch=fgetc(fp)) != EOF){
        if(ch=='\n') {
            h++;
            nl=1;
        } else {
            buf[i++]=(char)ch;
            if(nl==0) w++;
        }
    }
    INFO("READ %i char", i);
    TRACE("\n%s", buf);
    rolls = count_rolls(buf, w, h);
    INFO("Total amount of rolls is %i", rolls);

    return 0;
}
