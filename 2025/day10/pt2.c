#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>

/*
   Greatest Common Deominator example maybe helpful.. 
   gcd(12,4) - returns gcd(4,8) which returns gcd(8,4) that is gcd(4,4) which
   becomes 4! GCD is 4! 4*3
*/

int gcd(int a, int b){
    if(a==b) {
       return a;
    } else
    if(a>b) {
        return gcd(b,(a-b));
    } else 
        return gcd(b,a);
}

/* pt2 helper function - what happens when you push a button */
static void push_button(int button/* as bitfield */, int *joltage, int N /* how many places */ ){
    /* We need to interprite the bitfield and add on to the joltage */
    int n = button; 
    while(N--)
    {
        DEBUG("button: %i",n);
        joltage[N] += (n&1);
        n>>=1;
    }
}
static int compare_jolt(int *jolt_ref, int *jolt_mine, int N /* Amount of bits or spaces in the array, 1-indexes */)
{
    while(N--)
    {
        if(jolt_ref[N]==0 || jolt_mine[N] == 0) return -1; // Error occured - not the same
        if(jolt_ref[N]!=jolt_mine[N]) return 0; // Not the same, can be used in a condition
    }
    return 1; // never failed
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
        if(str[i]=='(' || str[i]==','){
            i++;
            continue;
        }
        indexes = str[i] - '0';
        if(indexes >= bits) 
            return -1;

        bitshifts = (bits-1)-indexes;
        start |= 1 << bitshifts;
        i++;
    }
    return start;
}

/* Does the heavy lifting and returns the amount of presse each line needs */
static int parse_line(char *line){

    /* This is the array we use to create the actual bit representation of the num*/
    int lights_arr[256] = {0};
    int joltage_arr[256] = {0};
    DEBUG("%s",line);

    int i=0,l_i=0, l=0;     

    char c;
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
        if(t[0]=='{') break;
        buttons[buttons_count++]=create_bitfield(t, l);
        DEBUG("%s", t);
    }
    DEBUG("%s tokenized", t);
    strcpy(tmp,t); 
    DEBUG("%s ready to tokenize", tmp);
    int jolt_i=0;

    /* I dont know if it will help later or not, but I at least identified a range. At worst, 
     * each button increments only one index which means the sum of all the numbers is the most
     * we will ever need to push a button - but at the same time, the highest number in the list
     * is the minimum we need to push, even if we have a magic button that hits
     * all numbers, the highest number must still be reached.
     * Extracting them for now. Lets see.
     */
    int MIN_LIMIT=0;
    int MAX_LIMIT=0;
    for (t = strtok(tmp, "{,"); t; t = strtok(NULL, ",}")) 
    {
        int jolt = atoi(t);
        if(jolt>MIN_LIMIT) { MIN_LIMIT=jolt; }
        MAX_LIMIT+=jolt;

        joltage_arr[jolt_i] = jolt; 
        DEBUG("%d",joltage_arr[jolt_i]);

        jolt_i++;
    }

    /* pt2 has slightly different logic, and now we dont care about the lights - 
     *  now its all about the joltage
     *      -ignored-       Buttons with indecies       0 1 2 3  <--- Index
     *       [.##.]    (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7} <---- Amount of times this index is pushed
     *
     * So we create a joltage_arr {0,0,0,0,0} and a function to use buttons to increment `push_button`
     *
     * Its a goddamn linalg problem!!!
     *
     * MAT1100 really comes in handy now!
     *
     * So what we have are parts to a whole. The numbers are literally:
     * (3)   (1,3)    (2)  (2,3)  (0,2)  (0,1) 
     *
     * x_1*B0 + x_2*B1 + x_3*B2 + x_4*B3 = Target 
     *
     * x_1+x_2+x_3+x_4 = (7, 20]
     *
     * Its just a matrix!! 
     *
     *   (0 0 0 1)*x_1  This can be written as 
     * + (0 1 0 1)*x_2 
     * + (0 0 1 0)*x_3 
     * + (0 0 1 1)*x_4 
     * + (1 0 1 0)*x_5 
     * + (1 1 0 0)*x_6 
     * = {3,5,4,7}
     *
     *
     * We just need to solve for the variables! Damnit, its not a square, than removes a lot
     * of matricies tricks i know
     *
     */

    /* This is the one i will build and check with the reference */
    int joltage[256]={0};

    int best=0;
    INFO("Upper limit is %i, lower limits is %i", MAX_LIMIT, MIN_LIMIT);

    free(t);
    DEBUG("best press is %i", best);
    return best;
}

int pt2(FILE* fp){

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

    free(line);
    return 0;
}
