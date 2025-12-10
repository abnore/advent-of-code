#include "main.h"
#include <blackbox.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int pt1(FILE* fp){

    int split=0;
    ssize_t line_len=0;
    int start_i = 0; 
    char *curr_line = NULL;     // reading in to current line
    char *prev_line = NULL;     // remembering the previous line
    size_t cap = 0;             // capacity for line (how much allocated)

    // and i know this technically leaks memory, but who cares
    // Read in the first line and find the 'S'

    while((line_len = getline(&curr_line, &cap, fp)) > 0)
    {
        if(curr_line[line_len-1]=='\n') {
            curr_line[line_len-1]='\0';
        }

        if(prev_line == NULL){
            DEBUG("line length=%zu, including \\n", line_len);

            prev_line = realloc(prev_line, sizeof(char)*line_len);
            while((curr_line[start_i])!='S') {
                start_i++;
            }

            TRACE("S is at index [%i] in curr_line: %c", start_i, curr_line[start_i]);

            memcpy(prev_line, curr_line, line_len); 
            continue;
            /* The first line starts as a previous line */
        }         
        
        if(start_i){
            curr_line[start_i] = '|';
            start_i = 0;
        } else {
            // Here is where we will look for the ^ and start splitting and counting
            for(int i=0; i<line_len;++i){
                if(prev_line[i]=='|' && curr_line[i]=='^')
                {
                    if(i>0) curr_line[i-1]='|';
                    if(i<line_len-1) curr_line[i+1]='|';
                    split++;
                } else
                if(prev_line[i]=='|' && curr_line[i]=='.')
                {
                    curr_line[i]='|';
                }
            }
        }
        TRACE("The previous line is %s", prev_line);
        DEBUG("The current line is %s", curr_line);

        memcpy(prev_line, curr_line, line_len); 
    }

    INFO("Amount of time we split is %i", split);

    return 0;
}


/* 
 * The same program, only I went code golfing a bit
 *
 *  #include "main.h"
 *  #include <stdlib.h>
 *  int pt1(FILE* fp){
 *   int s=0,S=0,i=0;char *c=0,*p=0;size_t k=0;ssize_t l;
 *   while((l=getline(&c,&k,fp))>0){
 *    c[l-=c[l-1]==10]=0;
 *    if(!p){p=malloc(l);
 *     for(;c[S]!=0x53;S++);
 *     for(;i<l;p[i]=c[i],++i);
 *     continue;}         
 *    if(!S){for(i=0;i<l;++i) if(p[i]==0x7c) 
 *     c[i]==0x5e?(i&&(c[i-1]=0x7c),i<l-1&&(c[i+1]=0x7c),s++):
 *     c[i]==0x2e&&(c[i]=0x7c);
 *    }else c[S]=0x7c; S=0;
 *    for(i=0;i<l;p[i]=c[i],++i);}
 *   printf("%d\n",s);return 0;}
 */
