#include "main.h"
#include <blackbox.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int pt2(FILE* fp){

    uint64_t *curr=NULL;
    uint64_t *next=NULL;
    uint64_t time_lines=0;

    int cols = 0; // since line_len becomes -1 at the end, we store the length
    int S = 0;    // index for the S which begins the beams, and check for first line

    char *line = NULL;          // reading in to current line
    size_t cap = 0;             // capacity for line (how much allocated)
    ssize_t line_len=0;
    
    // and i know this technically leaks memory, but who cares
    // Read in the first line and find the 'S'
    while((line_len = getline(&line, &cap, fp)) > 0)
    {
        if(line[line_len-1]=='\n') {
            line[--line_len]='\0';
        }
        /* When this becomes -1 it will not reach here */
        cols = (int)line_len;
        
        if(S==0) {
            curr = calloc(cols, sizeof(uint64_t)); // So we can 0-init them
            next = calloc(cols, sizeof(uint64_t));

            while((line[S])!='S') S++; // just find the s
            curr[S] = 1; // The S begins one timeline!, counted so we skip
            continue; // The current line we KNOW is 1 timeline, we skip it
        } 
        /*
           Here is where we will look for the ^ and start splitting and counting
           Part 2 does not concern itself with beams anymore. Now its timelines.
           I was briefly concidering a DFS, until i realized that this problem
           is purely numeric. We are only concerned about timelines as an amount
           and that it sums over time - and only splitters can create them.
           If 3 timelines comes into a splitter we add that on to whatever was there
           from previous time lines, column-wise 

        */

        for(int i=0; i<cols;++i){
            if(curr[i] == 0) 
                continue; // Dont care about no branches
            if(line[i] == '.')
                next[i] += curr[i]; // just move the number along - no splitting here
            if(line[i] == '^'){
                next[i-1] += curr[i]; // Here we need to add the timelines on to whatever
                next[i+1] += curr[i]; // amount is here from before - branching through
            }
        }

        DEBUG("The curr line is %s", line);

        // We need to move next into curr and reset next
        memcpy(curr, next, cols*sizeof curr);
        bzero(next, cols*sizeof next);
    } 

    DEBUG("cols=%i", cols);
    /* Simply sum up all the time lines */
    for(int i=0; i<cols;++i)
    {
        TRACE("time_lines[%i] is %llu", i, curr[i]);
        time_lines += curr[i];
    }

    INFO("Amount of time_lines is %llu",time_lines);

    return 0;
}

