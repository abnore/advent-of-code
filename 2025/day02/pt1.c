#include "main.h"
#include <blackbox.h>
#include <math.h> // Required for log10 and floor
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


static void print_ranges(range* ranges, int amount) {
    for(int i = 0; i < amount; ++i) {
        printf("%i ==> I: %-11lli  II: %lli\n",i+1, ranges[i].start, ranges[i].end);
    }
}

static int split_into_ranges(char* str, range* ranges)
{
    char* ptr = str;
    int amount= 1;
    while (*ptr != '\0') {
        if (*ptr == ',')
            amount++;
        ptr++;
    }
    INFO("We found %i ranges", amount);

    char first[128] = {0}, second[128] = {0};
    int skip=0, f=0; // flags for which we are reading
    int idx1 = 0;
    int idx2 = 0;

    ptr = str;

    for (int i = 0; i < amount; ++i)
    {
        f = 0;
        idx1 = idx2 = 0;

        int safety = 0;
        while(*(ptr) != '\n')
        {
            safety++;
            if(*ptr == '-'){
                f=1;
                skip = 1;
            } else if(*ptr == ','){
                skip=1;
                break;
            } else {
                skip=0;
            }

            if (f==0 && skip==0){
                first[idx1++] = *ptr;
            } else if (f==1 && skip==0) {
                second[idx2++] = *ptr;
            }

            ptr++;

        }// Reached a comma, therefore we have a new range

        // basicly zeroing out the rest of the number, if there is junk
        first[idx1]='\0';
        second[idx2]='\0';
        ptr++;

        ranges[i].start = atol(first); // atoi overflowed
        ranges[i].end = atol(second);

    }
    return amount;
}

static int amount_digit(int64_t n)
{
    if (n == 0) return 1;
    return floor(log10(n)) + 1;
}

static int64_t check_sequence(int64_t num, int64_t top)
{
    // Can ignore all the numbers with odd digits (123 etc..)
    int amount = amount_digit(num);
    int am_top = amount_digit(top);
    if(!(amount%2==0)) return 0;

    //convert it back to a string to check length and sameness
    char str[40] = {0};
    sprintf(str, "%lld", num);

    int len = strlen(str);

    if(amount == am_top && (am_top%2)) return -1;
    int idx = 0;
    int mid_idx = len/2;


    while(idx < len/2)
    {
        if(str[idx++]==str[mid_idx++]) continue;
        return -1;
    }
    return num ;
}

static int64_t walk_through(range *ranges, int amount)
{
    int64_t sum=0;
    // If the number is odd, skip it
    for(int i = 0; i < amount; ++i)
    {
        int64_t num1 = ranges[i].start;
        int64_t num2 = ranges[i].end;
        DEBUG("Checking first range %lli - %lli", ranges[i].start, ranges[i].end);
        while(num1 <= num2){

            int64_t res = check_sequence(num1, num2);
            if(res > 0) sum += res;
            num1++;
        }
    }

    return sum;
}
int pt1(FILE* fp){

    char str[32000];
    fgets(str, sizeof(str), fp);
    /* Now i have stored the entire file in str
     * Now we parse */
    INFO("Input is:\n%s", str);
    range ranges[64];

    int amount = split_into_ranges(str, ranges);
    //print_ranges(ranges, amount);

    INFO("We have %i ranges", amount);

    int64_t sum = walk_through(ranges, amount);

    INFO("The final number is %lli", sum);
    return 0;
    return 0;
}
