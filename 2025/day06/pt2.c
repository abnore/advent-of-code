#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int pt2(FILE* fp)
{
    /* Reading file into buffer, and getting width and height*/
    char buffer[32000];
    size_t pos = 0;

    int c;
    int w=0,h=0,n=0;

    while ((c = fgetc(fp)) != EOF) {
        if(c=='\n'){
            if(h==0)w=n;
            h++;
            continue;
        }
        buffer[pos++] = (char)c;
        n++;
    }
    buffer[pos] = '\0';

    /*
     Pt2 its different, now i have to start at the end ( wish i had tac )
     and construct the numbers, one char at a time, start at the top, and going down
     the column. Therefore at the most right its 0 0 4 as one num, and then 4 3 1 etc

        123 328  51 64-
         45 64  387 23-
          6 98  215 314
        *   +   *   +
                     ^^
                     ||
                     40
                     30
                     14
      The rightmost problem is 4 + 431 + 623 = 1058

      First its idx 0 for each line, so thats l[0], l[1], etc. which equates to
      num idx+w*0, idx+w*1, idx+w*2 etc.. Idx is the inrow ref, and w*x is the column
    
      Now the buffer comes in handy! so i will count from the end of the line,
      work my way backwards, and the top num is MSC, therefore i should be able
      to just construct the num straight into an int remember counting is 1
      based, so w-1 for column num etc*/

    int op_row = h - 1;          // operator row
    int num_rows = h - 1;        // rows containing digits
    int num_cols = w - 1;        // rows containing digits
    int num_count = 0;
    char digits[128]={0}, op;         // container for the nums, and operations
    uint64_t stack[128]={0};

    uint64_t final_score=0;

    for(int col=num_cols; col>=0; --col) // This is every char (col), starting at the end
    {
        int dpos = 0;  // index for collecting digits only

        /* Forst row 0, column 15, then we move to
         * row 1, column 15 etc etc.. idx + nums_per_line*l;
         * row 2, colum 15
         * Relative placement is then row*nums_per_lines + col
         */
        for(int row=0; row<num_rows; ++row) // This is for every lines (row),
        {
            char ch = buffer[row*w + col]; //relative digit inside the column
            // ONLY store digits into temp
            if(ch >= '0' && ch <= '9')
                digits[dpos++] = ch;
        }
        digits[dpos] = '\0';  // null-terminate
        if (dpos > 0) {
            uint64_t value = strtoull(digits, NULL, 10);
            stack[num_count++] = value;
            TRACE("col=%i -> number %llu", col, value);
        }        

        op = buffer[w*op_row+col]; // Extracting the operator at the end

        // The operators is the marker for the sequence being done!
        if ((op == '+' || op == '*') && num_count > 0) {

            uint64_t acc = stack[0];
            for (int i = 1; i < num_count; i++) {
                if (op == '+') {
                    TRACE("%llu + %llu = %llu", stack[i-1], stack[i], (stack[i-1]+stack[i]));
                    acc += stack[i];
                }
                else {
                    TRACE("%llu * %llu = %llu", stack[i-1], stack[i], (stack[i-1]*stack[i]));
                    acc *= stack[i];
                }
            }

            DEBUG("Problem result (%c): %llu", op, acc);
            DEBUG("+ %llu = %llu", final_score, final_score+acc);
            final_score += acc;

            num_count = 0; // reset for next group
        }
    }
    INFO("Final score is %llu", final_score);

    return 0;
}

