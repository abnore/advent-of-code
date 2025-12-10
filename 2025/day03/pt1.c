#include "main.h"
#include <blackbox.h>
#include <stdio.h>
#include <stdlib.h>

static int parse_line(char *line, int* num_lines)
{
    DEBUG("Parsed line: %s", line);
    (*num_lines)++;

    // Two pointers - one for each number i am looking for
    char *ptr1, *ptr2;
    ptr1 = line;
    ptr2 = line+1;

    // Storing the second highest number, highest will be ptr1
    char second_highest = *ptr2;

    // Going through each character in the line - moving the second pointer
    while(*ptr2 != '\n')
    {
        // If the second pointer is bigger then 1, update both
        if(*ptr2 > *ptr1 && *(ptr2+1) != '\n') {
            ptr1 = ptr2;
            second_highest = *(++ptr2);
        } else // otherwise only update second biggest
        if(*ptr2 > second_highest) {
            second_highest = *ptr2;
            ptr2++;
        } else { // increment the pointer
            ptr2++;
        }
    }

    // Construct the final stack
    char jolt[3] = { *ptr1, second_highest, '\0' };

    DEBUG("jolt is %s", jolt);
    return atoi(jolt);
}

int pt1(FILE* fp)
{
    char *line = NULL;
    size_t cap = 0;
    int num_lines=0;
    int sum = 0;
    while (getline(&line, &cap, fp) != -1) {
        sum += parse_line(line, &num_lines);
    }
    free(line);
    DEBUG("num lines is %i", num_lines);
    INFO("Total joltage: %i\n", sum);
    return 0;
}
