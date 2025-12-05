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
    range missing_ranges[256];
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
    // And now we have all the numbers in a csv buffer - now let us compare
    INFO("Parsed all the ranges. number of ranges is %i", num_ranges);
    free(line);

    /*
    * Maybe just keep score over the missing ranges?
    * Keep lowest and highest 0 and 0 at first then increase? Then i will need
    * to keep track of holes. So i will need missing ranges and highest....
    */

    /*
    * 3-5,  lowest 3 - and 5 highest
    * 10-14 lowest 3 and missing range 6-9 and 14 highest
    * 16-20 lowest 3 and missing range 6-9, 15, and 20 highest.
    * 12-18 lowest 3 and missing range 6-9, and 20 highest
    * So they check could be, does it contain any of the missing range? If so,
    * fill out the number and set highest....
    * 1) Is the lowest lower then the lowest?
    * 2) Is the highest higher then the highest?
    * 3) If yes to both - then this range is bigger then everything we have,
    *       extend low and high, done!
    *    if lowest then lowest, but not higher then higher, we overlap - check if cover
    *    missing range.
    *    if not lower then lowest, but higher then highest, check if lowest
    *    is higher then highest - if not we overlap - remove numbers! - otherwise we need
    *    to extend missing range
    * If no to either of these questions we are inside what we already have and
    * must fill missing range And we CANT have more missing ranges then ranges,
    * because even if perfecntly spread out its n-1 missing ranges. X-X-X, as
    *   X is what we have, - is between
    */

    uint64_t lowest=ranges[0].start; // has to be a starting point, 0 is always low
    uint64_t highest=0;
    /*range missing_ranges*/
    int num_missing_ranges=0;

    for(int i=0; i<num_ranges;++i)
    {
        // Should check if the range is filling out some missing ranges perhaps
        // 1) new range - check if lowest is bottom, or above the previous ranges
        if(ranges[i].start < lowest) {
            lowest=ranges[i].start;
        } else if(ranges[i].start > highest {
            missing_ranges[i].end = ranges[i].start-1; // Missing range is up to this num-1
            missing_ranges[i].start = highest+1; // There cant be anything else in this space
        }
        if(ranges[i].end > highest) {
            highest=ranges[i].end;
        }
        // THIS IS INCOMPLETE - WIP!!
    }



    return 0;
}
