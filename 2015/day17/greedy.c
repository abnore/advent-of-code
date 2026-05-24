#include "main.h"
#include <blackbox.h>

#define target  25 // test

/*
    20 15 10 5 5
    ^   .

  */

int greedy(FILE* fp)
{
    int nums[32]={0};
    int num=0;
    while( fscanf(fp, "%i\n", &nums[num]) != EOF ) num++;

    int count=0;
    for(int head=0; head<num; head++){
        int h = nums[head];
        printf("head value %i\n", h);

        for(int tail=head+1; tail<num; tail++){
            printf("adding %i = ", nums[tail]);
            int t = nums[tail];
            h += t;
            printf("%i", h);

            if( h > target) {
                h -= t;
                printf(" => Too high \n");
                continue;
            }

            if (h == target){
                printf(" => Got it! \n");
                h -= t;
                count++;
                continue;
            }
            printf("\n");
        }
    }

    printf("done\n");

    return count;
}
