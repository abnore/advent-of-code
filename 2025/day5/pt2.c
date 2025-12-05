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
* First range is 5,513,712,084,434 numbers... And going 3 loops deep on that, no way
*
* Need to check uniqueness differently. Can only check the ranges end points.
* New range has to fit between, otherwise i have to remove the number, and check again
* This way its O(n^2) instead - or close too it
* - i need to merge. Maybe merge sort?
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

    while((ret = getline(&line, &linecapp, fp))>1){
        char num1[64];
        char num2[64];
        if(line[ret-1]=='\n') ret--; //remove the \n for now

        int first=0;
        for(int i=0;i<ret;++i){
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
                DEBUG("line[i] is %c num1[y] is %c", line[i], num2[idx2]);
                idx2++;
            } else {
                ASSERT(0, "unreachable - WTF");
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
    INFO("Parsed all the ranges. number of ranges is %i", num_ranges);
    free(line);

    /*
    * Maybe just keep score over the missing ranges?
    * Keep lowest and highest 0 and 0 at first then increase? Then i will need
    * to keep track of holes. So i will need missing ranges and highest....
    */
    /*
    * Too time consuming to keep track manually and check - its cumbersome, and
    * I am missing a more elgant solution. The ranges, should be possible to merge
    * But in that case i would need to sort them, and if they overlap right next to
    * eachother i can just turn them into one range. I will need a sorting function
    * Then a dynamic array perhaps.
    */

    /*   0 1 2 3
        |i| | | |
        | |j| | |  -After this, the smallest is 0

        | |i| | |
        | | |j| |
*/
    range sorted_ranges[264];

    // Ended up with a bubble / selection sort type of algorithm to simplify merging
    uint64_t temp_start;
    uint64_t temp_end;
    for(int i=0; i<num_ranges-1;++i){
        DEBUG("i - ranges[%i].start is %llu",i, ranges[i].start);
        for(int j=i+1;j<num_ranges;++j){
            DEBUG("j - ranges[%i].start is %llu",j, ranges[j].start);
            if(ranges[j].start < ranges[i].start)
            {
                temp_start  = ranges[i].start;
                temp_end    = ranges[i].end;
                ranges[i].start = ranges[j].start;
                ranges[i].end = ranges[j].end;
                ranges[j].start = temp_start;
                ranges[j].end = temp_end;
            }
        }
    }

    /*
NOTES: Just walking through an example,
index| start         end         index (moving down)
  0  |   3            5             merged=0       (i=1)   First range - nothing
  1  |  10    (14)(18)20            merged=1       (i=2) Is 14 >= then 12? new->18
  2  | (12)23        (18)40         merged=1       (i=3) is 18 >= 16 = new -> 20
  3  | (16)          (20)           merged=1       (i=4) is 20 >= 23? no - merged moves, overwrites where i was
  4  | (23)          (40)           merged=2       (i=5) is 40 >= 28? yeah - but 40 > 38 so its absorbed
  5  | (28)          (38)           merged=2       (i=6)  - etc

    */

    // Just merging ranges in one pass
    uint64_t total_sum = 0;
    int merged = 0;
    for(int i=1; i<num_ranges;++i) // Full pass through the ranges
    {
        DEBUG("ranges[merged] (%llu-%llu) -- ranges[i] (%llu-%llu)", ranges[merged].start, ranges[merged].end, ranges[i].start, ranges[i].end);

        if(ranges[merged].end >= ranges[i].start)
        {
            if(ranges[merged].end >= ranges[i].end)
            {
                // Do nothing here - we leave it and overwrite it
                DEBUG("This range is completly inside the previous");
            } else {
                ranges[merged].end = ranges[i].end;
            }
        } else {
            // whenever we move merged we need to insert the current i,
            // overwriting whatever range it currently is at. Merged is therefore
            // accumulating the ranges.
            merged++;
            ranges[merged].start = ranges[i].start;
            ranges[merged].end = ranges[i].end;
        }
        DEBUG("Total merged is %i", merged);
    }
    merged++; // One last increment to count the last one

    // And summing it up at the end - total number is just the difference+1 (inclusive)
    for(int i=0; i<merged;++i)
    {
        DEBUG("Ranges[%i]: start %llu   end %llu", i, ranges[i].start, ranges[i].end);
        total_sum += ranges[i].end - ranges[i].start + 1;
    }

    INFO("Total sum is %llu", total_sum);

    return 0;
}
