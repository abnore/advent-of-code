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
static int parse(int *nums, int len, int n_idx, int sum, int *ways, int lines)
{
    int ret=0;
    for (int i=n_idx; i<len;i++){
        int next = nums[i];
        int so_far = sum + next;
        if (so_far > target) continue;
        if (so_far == target)
        {
            ways[lines+1] += 1;
            ret++;
            continue;
        }
        ret+=parse(nums, len, i+1, so_far, ways, lines+1);
    }
    return ret;
}

int pt2(FILE* fp)
{
    // read the numbers in the buffer
    int containers[32]={0};
    int ways[32] = {0};
    int amount=0;
    while( fscanf(fp, "%i\n", &containers[amount]) != EOF ) amount++;
    printf("amount %i\n", amount);

    // we need to check every single combination of numbers
    parse(containers,amount, 0, 0, ways, 0);

    int count=0;
    for(int i=0; i<amount-1; i++) {
        if(ways[i]) {
            count=ways[i];
            break;
        }
    }

    return count;
}

