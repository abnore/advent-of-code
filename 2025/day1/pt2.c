#include "main.h"
#include <blackbox.h>
#include <stdlib.h>


static void left_rotate(int clicks, int* nz, int* start)
{
    // if start 2, clicks 3
    while(clicks){
        if(*start==0){
            (*nz)++;
        }
        (*start)--;
        clicks--;
        if(*start<0) *start = 99;
        DEBUG("start=%i, clicks=%i, nz=%i", *start, clicks, *nz);
    }
}

static void right_rotate(int clicks, int* nz, int* start)
{
    while(clicks){
        if(*start==0){
            (*nz)++;
        }
        (*start)++;
        clicks--;
        if(*start > 99) *start = 0;
        DEBUG("start=%i, clicks=%i, nz=%i", *start, clicks, *nz);
    }

}

static int parse_commands(FILE *fp)
{
    // Buffer to store each line of the file.
    int start = 50;
    char line[256];
    int num_zero = 0;

    // Read each line from the fp and store it in the
    // 'line' buffer.
    while (fgets(line, sizeof(line), fp)) {
        // Print each line to the standard output.
        if (line[0]=='R'){
            line[0]='0';
            int res = atoi(line);
            DEBUG("start=%i, res=%i, nz=%i",start, res, num_zero);
            right_rotate(res, &num_zero, &start);

        } else if (line[0]=='L'){
            line[0]='0';
            int res = atoi(line);
            DEBUG("start=%i, res=%i, nz=%i",start, res, num_zero);
            left_rotate(res, &num_zero, &start);
        }
        //            if(start==0){
        //                num_zero++;
        //            }

    }
    return num_zero;
}

int pt2(FILE* fp){
    int sum = parse_commands(fp);
    INFO("The answer is %i", sum);
    INFO("Hello from pt2");
    return 0;
}
