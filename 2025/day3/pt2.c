#include "main.h"
#include <blackbox.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t solve_line(char *line, int digits)
{
    int len = strlen(line);
    char *p = line; // pointer to look at the chars
    char stack[1024]; // to preserve the numbers while we walk the line
    int top = 0;

    // We are going to need to shave off some numbers, -1 for '\n'
    int digits_to_cut = len-digits-1;

    DEBUG("digits to cut %i", digits_to_cut);

    // Plan: Walk the line - the biggest number is stored in the stack, inc top,
    // the second highest number will be stored, and overwrites the previous
    // number if its bigger. Every number that is higher than a previous number
    // can cut up to digits_to_cut, we can drop numbers between, but only until its
    // not lower then the number before
    while(*p != '\n')
    {
        // While i am not at the first line and the previous digit is smaller AND i can remove
        // Then backtrack - but the pointer stays the same - only the stack we edit
        while (top > 0 && stack[top-1] < *p && digits_to_cut > 0 ){
            top--;
            digits_to_cut--;
        }
        stack[top++] = *(p++); // Add and increment the pointer for the whole line
    }

    top = digits;
    stack[top] = '\0'; // need to shave off possible garbage, may be up to 1024 chars
    DEBUG("stack = %s", stack);
    return atoll(stack);
}

int pt2(FILE* fp)
{
    char *line = NULL;
    size_t cap = 0;

    uint64_t sum = 0;

    while (getline(&line, &cap, fp) != -1) {
        sum += solve_line(line, 12); // 12 digits instead of 2
    }

    INFO("Total joltage: %llu\n", sum);

    free(line);

    return 0;
}
