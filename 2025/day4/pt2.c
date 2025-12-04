#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int count_neighbours(char* buf, int w, int h, int pos )
{
    int neighbours=0;
    // Loop through all possible neighbours and check value. If . no, if @ +1
    int rel_x = pos % w; // Relative positions in the row 0-w
    int rel_y = pos / h; // Actual column number 0-h

    for(int y=-1; y<=1; y++)
    {
        int new_col = rel_y + y;
        if (new_col < 0 || new_col >= h ) continue;

        for(int x=-1; x<=1; x++)
        {
            int new_row = rel_x + x;

            if(x==0 && y==0) continue; // This is me, dont count
            if(new_row < 0 || new_row >= w) continue;

            int n_pos = (new_col*w) + new_row; // recreate index in the flat buffer

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
    int total = w*h;
    int rolls = 0;
    int num_adjacent=0;
    // New buffer created by "removing" the rolls. This overwrites the buf,
    // Important that it doesnt happen during, as that will mess up the counting
    char buf_new[32000] = {0};
    memcpy(buf_new, buf, (size_t)w*h);

    for(int i=0; i <total; ++i)
    {
        if(buf[i] =='@'){
            num_adjacent = count_neighbours(buf, w, h, i);
            if(num_adjacent < 4)
            {
                rolls++;
                buf_new[i] = '.'; // Create the updated buffer
            }
        }
    }
    memcpy(buf, buf_new, (size_t)w*h);
    return rolls;
}

int pt2(FILE* fp)
{
    char buf[32000]={0};
    int rolls=0;
    int ch, i=0, w=0, h=0, nl=0;
    // Read in the contents of the file, and remove the \n
    // Also count the width, height and total stored i=h*w
    while((ch=fgetc(fp)) != EOF){
        if(ch=='\n') {
            h++;
            nl=1;
        } else {
            buf[i++]=(char)ch;
            if(nl==0) w++;
        }
    }
    DEBUG("READ %i char, width: %i height: %i", i, w, h);
    TRACE("\n%s", buf);
    int ret;
    while((ret = count_rolls(buf,w,h)) != 0 ){
        rolls += ret;
        DEBUG("Total amount of rolls is %i", rolls);
    }
    INFO("FINAL amount of rolls is %i", rolls);
    return 0;
}
