#include "main.h"
#include <blackbox.h>
#include <stdlib.h>


static void left_rotate(int clicks, int* start)
{
    // if start 2, clicks 3
    while(clicks){
        *start-=1;
        if(*start<0){
            *start=99;
        }
        clicks--;
    }
}

static void right_rotate(int clicks, int* start)
{
    while(clicks){
        *start+=1;
        if(*start>99){
            *start=0;
        }
        clicks--;
    }

}

static int parse_commands(FILE *fp)
{
    int start = 50;
    int num_zero = 0;
    // Buffer to store each line of the file.
    char line[256];

    // Check if the file was opened successfully.
    if (fp != NULL) {
        // Read each line from the fp and store it in the
        // 'line' buffer.
        while (fgets(line, sizeof(line), fp)) {
            // Print each line to the standard output.
            if (line[0]=='R'){
                line[0]='0';
                int res = atoi(line);
                right_rotate(res, &start);

            } else if (line[0]=='L'){
                line[0]='0';
                int res = atoi(line);
                left_rotate(res, &start);
            }
            if(start==0){
                num_zero++;
            }

        }
    }
    return num_zero;
}

int pt1(FILE* fp){
    int sum = parse_commands(fp);
    INFO("The answer is %i", sum);
    INFO("Hello from pt1");
    return 0;
}
