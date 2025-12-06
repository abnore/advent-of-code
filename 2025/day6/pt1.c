#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int pt1(FILE* fp)
{
    /* Reading file into buffer, and getting widht and height on table*/
    char buffer[32000];
    size_t pos = 0;

    int c;
    int w,h, n;
    int num_space;
    while ((c = fgetc(fp)) != EOF) {
        buffer[pos++] = (char)c;
        if(c=='\n')h++;
        n++;
    }
    w=n/h;
    buffer[pos] = '\0';
    rewind(fp);

    TRACE("\n%s", buffer);
    INFO("OK: i know there is %i lines of numbers, each line is %i char wide", (h-1), w);
    INFO("The line of operators is index [%i]", h-1);
    INFO("h=%i, w=%i, n=%i", h, w, n);

    /* Converting every number into int and separating lines by -1 */
    int nums[4096];

    int num_idx=0;
    char* line=NULL;
    size_t cap=0;
    for(int i=0;i<h-1;++i){
        getline(&line, &cap,fp);

        char tmp[4096];
        strcpy(tmp, line);
        for (char* t = strtok(tmp, " "); t; t = strtok(NULL, " \n")) {
            nums[num_idx++]=atoi(t);
        }
    }
    getline(&line, &cap,fp);
    char operators[4096];
    int op_idx=0;
    strcpy(operators, line);
    for (char* t = strtok(operators, " "); t; t = strtok(NULL, " \n")) {
        DEBUG("These are the operators %s [%i]", t, op_idx);

        if(strcmp(t,"*")==0) operators[op_idx++]='*';
        if(strcmp(t,"+")==0) operators[op_idx++]='+';
    }

    int nums_per_line = num_idx/(h-1);

    INFO("There are %i nums per line", nums_per_line);

    char ops[2] = {'*','+'};


    /* I now have all the numbers, all the operators and the buffer for reference
     * Now i will go through every line, and for every position i will do the operator

        123 328  51 64
         45 64  387 23
          6 98  215 314
        *   +   *   +

        123 * 45 * 6 = 33210
        328 + 64 + 98 = 490
        51 * 387 * 215 = 4243455
        64 + 23 + 314 = 401

        First its idx 0 for each line, so thats l[0], l[1], etc. which equates to
        num idx+w*0, idx+w*1, idx+w*2 etc.. Idx is the inrow ref, and w*x is the column
     */

    uint64_t res[1024]={0};
    uint64_t final_res = 0;
    int mul=0, add=0;
    for(int idx=0;idx<=nums_per_line-1;idx++) { // this is every index -> moving right
        for(int l=0;l<h-1;l++){ // this is every line - moving down
            int rel_num=idx + nums_per_line*l;
            char op=operators[idx];
            if(op=='*'){
                mul=1;add=0;
            } else if(op=='+'){
                add=1;mul=0;
            }
            if(res[idx]==0) res[idx]=nums[rel_num];
            else {
                if(mul)
                    res[idx]*=nums[rel_num];
                if(add)
                    res[idx]+=nums[rel_num];
            }

            DEBUG("rel_num=%i, num=%i with op=%c",rel_num, nums[rel_num], op);
        }
        DEBUG("res[%i]=%llu", idx, res[idx]);
        final_res += res[idx];
    }

    INFO("The answer is: %llu", final_res);
    return 0;
}
