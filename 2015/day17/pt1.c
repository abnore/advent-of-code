#include "main.h"
#include <blackbox.h>

/*
    20 15 10 5 5
    ^   .

    First num, add the next. to large, add the next, bingo, done, too little,
    recurse from that num on!
  */

// #define target 25 // testing
#define target 150
int parse(int *nums, int len, int n_idx, int sum)
{
    printf("getting %i\n",sum);
    int ret=0;

    for (int i=n_idx; i<len;i++){
        int next = nums[i];
        int so_far = sum + next;

        printf("adding %i => ", next);
        if (so_far > target) {
            printf("Too large, next num\n");
            continue;
        }

        if (so_far == target) {
            printf("BINGO\n");
            ret++;
            continue;
        }
        // if not large enough, start again from this point
        printf("Recursing\n");
        ret+=parse(nums, len, i+1, so_far);
    }

    printf("Returning from end of func\n");
    return ret;
}


int pt1(FILE* fp)
{
    // read the numbers in the buffer
    int containers[32]={0};
    int amount=0;
    while( fscanf(fp, "%i\n", &containers[amount]) != EOF ) amount++;

    for(int i=0; i<amount; i++) printf("%i\n",containers[i]);
    // we need to check every single combination of numbers

    int count = parse(containers,amount, 0, 0);

    return count;
}
