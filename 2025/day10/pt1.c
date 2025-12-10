#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>
/* My first thought was that this is xor logic over a bitfield - will try to manually toggle first */

/* For debugging purposes so i can visualize this  */
static void print_bin(int num){
    int msb=1;
    while(num>msb) 
        msb<<=1;

    if (msb > num) 
        msb >>= 1;

    while(msb > 0){
        int b = (num & msb);
        printf("%i ", b?1:0);
        msb>>=1;
    }
    printf("\n");
}
/* Helper to print out the array in binary looking way */
static void print_arr(int *arr, int n){
    for(int i=0; i<n; ++i){
        printf("%d ", arr[i]);
    }
    printf("\n");
}
/* parses the string inside '(' and ')' and returns a number.
 * eg (2,3) when bits is 4, will give 3 (b0011), while bits 5 would give 6 (b00110)  */
static int create_bitfield(char *str, int bits)
{
    int i=0;
    int indexes;
    int bitshifts;
    int start = 0; // 0 0 0 0 

    // string boundry is ( )
    while(str[i]!=')')
    {
        DEBUG("str[%i]=%c", i, str[i]);
        if(str[i]=='(' || str[i]==','){
            i++;
            continue;
        }
        indexes = str[i] - '0';
        bitshifts = (bits-1)-indexes;
        start |= 1 << bitshifts;
        DEBUG("bitshifts=%i, start = %i, indexes %i, str[%i]=%c", bitshifts, start, indexes, i, str[i]);
        i++;
    }
    return start;
}

int pt1(FILE* fp){

    char *line=NULL;
    size_t cap=0;
    ssize_t ret = getline(&line, &cap, fp);
    if(line[ret-1]=='\n'){
        line[ret-1]='\0';
    }

    int blanks_arr[16]={0};
    int lights_arr[16]={0};

    INFO("%s",line);
    // l is the length of the lights array, example l=4, therefore 0-3 is valid indexes
    // while i is the index into the line 
    char c; int i=0,l=0;     
    char buttons_str[16]={0};
    int button[128]={0};

    while((c=line[i]) != ']'){
        DEBUG ("c is %c", c);
        if(c=='.') l++;
        if(c=='#') lights_arr[l++]=1;
        i++;

    }
    
//    print_arr(lights,l);
//    print_arr(blanks,l);
//    print_bin(l);

    int lights = create_bitfield("(0,3)", 4);
    INFO("(0,3) 4 bits produced %i - should be 1001! 9", lights);

    return 0;
}
