#include "main.h"
#include <blackbox.h>
#include <stdint.h> //  uin64_t
#include <stdlib.h> //  free()

/*
* Does this number fall into one of the ranges?
* PLAN: Range x-y, take the id numbers, ( id >= x && id <= y) in range!
* because => is 1 in [3-5], well 1 < 3 so NOT in range
* is 8 in that range? 8 > 3 is so thats good, BUT 8 > 5 so no.
* So we need to check every number for >= then x and also <= than y
*
* For every number, check every range. When a number falls in a range, that
* number is good - continue
*/


int pt1(FILE* fp){

    //char range[32000];
    uint64_t ids[4096];
    int num_ids=0;
    range ranges[256];
    int num_ranges=0;

    // This is how getline does it - mallocs a line (if not already malloc'd)
    // linecapp is the capacity (malloc(cap)) and ret is how many char was read
    char* line = NULL;
    size_t linecapp=0;
    ssize_t ret; //= getline(&line, &linecapp, fp);

//    ret = getline(&line, &linecapp, fp);
//    INFO("%s and ret is %zi", line, ret);

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
    //uint64_t ids[4096];
    int idx=0;
    while((ret = getline(&line, &linecapp, fp))!=EOF){
        char num[64];
        if(line[ret-1]=='\n') ret--; // remove '\n'

        for(int i=0;i<ret;++i){
            num[idx]=line[i];
            idx++;
        }
        DEBUG("num %i is %s", num_ids, num);
        num[idx]='\0';
        ids[num_ids]=atoll(num);
        num_ids++;
        idx=0;
    }
    // only for test.in
//    ASSERT(ids[0]==1, "should be 1 is %llu", ids[0]);
//    ASSERT(ids[1]==5, "should be 5 is %llu", ids[1]);
//    ASSERT(ids[4]==17, "should be 17 is %llu", ids[4]);

    // And now we have all the numbers in a csv buffer - now let us compare
    INFO("Parsed all the ranges, and all the IDS. Ready to check");
    INFO("number of ranges is %i, number of IDs %i", num_ranges, num_ids);
    free(line);

    int valid_nums = 0;
    for(int i=0;i<num_ids;++i)
    {
        for(int r=0;r<num_ranges;++r)
        {
            if(ids[i] >= ranges[r].start &&
                ids[i] <= ranges[r].end)
            {
                DEBUG("Valid ID %llu, next number!", ids[i]);
                valid_nums++;
                break;;
            } else {
                DEBUG("%llu does NOT fall into the range", ids[i]);
            }
            // If here, then it falls within a range
            //INFO("Valid ID! %i", ids[i]);
        }
    }


    INFO("%i of the available ingredien IDs are fresh!", valid_nums);

    return 0;
}
