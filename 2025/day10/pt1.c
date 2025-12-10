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
    int start = 0; 

    // string boundry is ( )
    while(str[i]!=')')
    {
        TRACE("str[%i]=%c", i, str[i]);
        if(str[i]=='(' || str[i]==','){
            i++;
            continue;
        }
        indexes = str[i] - '0';
        if(indexes >= bits) return -1;
        bitshifts = (bits-1)-indexes;
        start |= 1 << bitshifts;
        TRACE("bitshifts=%i, start = %i, indexes %i, str[%i]=%c", bitshifts, start, indexes, i, str[i]);
        i++;
    }

    return start;
}

/* Does the heavy lifting and returns the amount of presse each line needs */
static int parse_line(char *line){

    /* This is the array we use to create the actual bit representation of the num*/
    char lights_arr[256];
    DEBUG("%s",line);
    /* l is the length of the lights array in bits, l_i is the index into the string
     * e.g l=4, therefore 0-3 is valid indexes
     * while i is the index into the line we are reading 
     */
    char c; 
    int i=0,l_i=0, l=0;     

    char tmp[256];
    strcpy(tmp, line); 
    char *t = strtok(tmp, " "); 
    int t_len=strlen(t);
    while(t_len--){
        c=t[i];
        TRACE("c is %c", c);
        if(c=='['){
            lights_arr[l_i++]='(';
        }
        if(c=='.') l++;
        if(c=='#') {
            char idx =(char) l+'0';
            lights_arr[l_i++]=idx;
            l++;
            lights_arr[l_i++]=',';
        }
        if(c==']') {
            lights_arr[l_i-1]=')';
            break;
        }
        i++;
    }

    /* Now we are ready to parse the rest of the line, store them as buttons and then check
     * permutations of the xor to the lights array */
    int buttons[128]; // the actual button we will xor to try an create lights
    int buttons_count=0;
    for (t = strtok(NULL, " "); t; t = strtok(NULL, " ")) {
        if(t[0]=='{') continue;
        buttons[buttons_count++]=create_bitfield(t, l);
        DEBUG("%s", t);

    }
//    print_arr(lights,l);
//    print_arr(blanks,l);
//    print_bin(l);
//    char *str_lit = "(0,1,3)";
    int lights = create_bitfield(lights_arr, l);
//    DEBUG("%s %i bits produced %i",lights_arr, l, lights);
//    for(int i=0; i<buttons_count; ++i)
//        print_bin(buttons[i]);
//    print_bin(lights);

    /* With the lights as a bitfield and all the buttons we are ready to iterate */

//    // Lets test this manually:
//    int xor_res=0;
//    printf("Testing against the test, to see if i understood it\n");
//    xor_res^=buttons[0];
//    xor_res^=buttons[1];
//    xor_res^=buttons[2];
//    printf("is xor_res == lights?: %i == %i\n", xor_res, lights);
//    xor_res=0;
//    xor_res^=buttons[1];
//    xor_res^=buttons[3];
//    xor_res^=buttons[5];
//    xor_res^=buttons[5];
//    printf("is xor_res == lights?: %i == %i\n", xor_res, lights);

    /* My logic works Now i will iterate through all the way to push one button, then all the ways
       to push 2 buttons, and finally all the ways to push all the buttons. Once i have the target state
       i break out and remember the lowest amount of presses

       For this puzzle as it relates to xor:
       - Pressing a button twice cancels it:
            b ^ b = 0 -> so pressed 0 times and pressed 2 times are the same.
            b ^ b ^ b = b -> so pressed 3 times and pressed 1 times are the same. 
            (5 pushed twice in the example because another button canceled a bit)
       - Order doesnt matter: 
            pressing A then B is the same final state as B then A: ((((0 ^ A) ^ B)) == ((0 ^ B) ^ A))
     */

    int best = -1;
    int total = 1 << buttons_count;   // number of all subsets ( in reality 1<<N - 1 but already 0 indexes

    for (int mask = 0; mask < total; mask++) 
    {
        // Count presses manually (number of bits set in mask)
        int presses = 0;
        for (int b = 0; b < buttons_count; b++) {
            if (mask & (1 << b))
                presses++;
        }

        // If we already found a better solution, skip this one
        if (best != -1 && presses >= best)
            continue;

        // Compute the XOR result of this subset
        int state = 0;
        for (int b = 0; b < buttons_count; b++) {
            if (mask & (1 << b)) {
                state ^= buttons[b];
            }
        }
        // Check if this subset matches the target lights
        if (state == lights)
            best = presses;
    }
    DEBUG("best press is %i", best);
    return best;
}

int pt1(FILE* fp){

    char *line=NULL;
    size_t cap=0;
    ssize_t ret;
    int total = 0;
    /* Created everything for 1 line, that worked so i moved it inside here 
     * and created parse_line that returns the best */
    while((ret = getline(&line, &cap, fp))>0){
        if(line[ret-1]=='\n'){
            line[ret-1]='\0';
        }
        int best= parse_line(line);
        total += best;

    }
    INFO("total presses is %i", total);

    return 0;
}
