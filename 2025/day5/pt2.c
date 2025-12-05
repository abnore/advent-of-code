#include "main.h"
#include <blackbox.h>
#include <stdint.h> //  uin64_t
#include <stdlib.h> //  free()

/*
* Part 2 should be easy - i have constructed the ranges, and now i can
* just go through each range, in a loop where i=range.start and goes until
* i< range.end and then count how many numbers there are.
* I already stored how many ranges i have!
* And for this part i can ignore the whole second half of the list and disregard
* all the IDs
*
* Except they overlap!
*
* PLAN: Create a stack, and for every number, check if it is in the stack
* NEW PLAN!: - brute force doesn't work, O(n^3) got too big.
* Need to check uniqueness differently. Can only check the ranges end points.
* New range has to fit between, otherwise i have to remove the number, and check again
* This way its O(n^2) instead - or close too it
*/


int pt2(FILE* fp){

    range ranges[256];
    int num_ranges=0;

    // This is how getline does it - mallocs a line (if not already malloc'd)
    // linecapp is the capacity (malloc(cap)) and ret is how many char was read
    char* line = NULL;
    size_t linecapp=0;
    ssize_t ret; //= getline(&line, &linecapp, fp);

    int idx1=0;
    int idx2=0;

    while((ret = getline(&line, &linecapp, fp))!=1){
        char num1[64];
        char num2[64];
        if(line[ret-1]=='\n') ret--; //remove the \n for now

        int first=0;
        for(int i=0;i<ret;++i){
            //if(line[ret-1]=='\n') line[ret-1]=','; // Comma seperated in buffer
            //range[y++]=line[i];
            if(line[i]=='-') {
                first=1;
                continue;
            }
            if(first==0){
                num1[idx1]=line[i];
                DEBUG("line[i] is %c num1[y] is %c", line[i], num1[idx1]);
                idx1++;
            } else if(first==1){
                num2[idx2]=line[i];
                DEBUG("line[i] is %c num1[y] is %c", line[i], num1[idx2]);
                idx2++;
            } else {
                ERROR("WTF");
            }

        }
        num1[idx1] = '\0';
        num2[idx2] = '\0';
        ranges[num_ranges].start=atoll(num1);
        ranges[num_ranges].end=atoll(num2);
        num_ranges++;
        idx1=0;
        idx2=0;
    }
    // only for test.in
//    ASSERT(ranges[0].start == 3, "should be 3, is %llu", ranges[0].start);
//    ASSERT(ranges[0].end == 5, "should be 5, is %llu", ranges[0].start);
//    ASSERT(ranges[2].start == 16, "should be 10, is %llu", ranges[2].start);
//    ASSERT(ranges[2].end == 20, "should be 14, is %llu", ranges[2].start);

    // Now we have the ranges in a csv buffer
    //range[total-1]='\0'; // overwrite the last comma

    // And now we have all the numbers in a csv buffer - now let us compare
    INFO("Parsed all the ranges, and all the IDS. Ready to check");
    INFO("number of ranges is %i", num_ranges);

    /* PSEUDO-CODE
    for each num in each range
        while not in stack, add to stack
        if in stack, ignore

        return amount in stack
    */

    uint64_t stack[4096];
    int total_num_ids=0;
    int num_in_stack=0;

//    for(int i=0; i<num_ranges;++i) // for every range
//    {
//        DEBUG("num_ranges %i, i=%i", num_ranges, i);
//        for(uint64_t c=ranges[i].start;c<=ranges[i].end;++c) // for num in range
//        {
//            DEBUG("%i range - num is %llu", i, c);
//            stack[num_in_stack++]=c;
//            total_num_ids++;
//            for(int s=0;s<total_num_ids;++s){ // for every num in stack except newly added
//                // starts empty - but then it checks a num
//                if(num_in_stack > 0 && c==stack[s-1]) {
//                    DEBUG("%llu in stack! skipping", c);
//                    num_in_stack--;
//                    break;
//                }
//                DEBUG("stack[%i]=%llu", s, stack[s]);
//            }
//        }
//    }
//    INFO("total num in stack %i", num_in_stack);
//    INFO("There should be %i of IDs", total_num_ids);
    free(line);
    return 0;
}
