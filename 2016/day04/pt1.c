#include "main.h"
#include <blackbox.h>
#include <regex.h>

/* Taking this challenge as a perfect oppurtunity to brush up on my REGEX skills
 * that i have neglected for far too long. I will set up a capture group for
 * everything up to the ID, which also gets to be a group. I will capture the
 * checksum, as well. Then i will work on the 3 groups
 *
 * https://regex101.com/ helps me create the regex, and the man page for regex
 * as well. This is a skill that can be very useful!
 * */

int pt1(FILE* fp){
    char * pattern = "^([a-z-]+)([0-9]{1,5})\\[([a-z]{5})\\]$";
    regex_t re; // The compiled expression
    regcomp(&re, pattern, REG_EXTENDED | REG_NEWLINE);

    char *line=NULL;
    size_t cap=0;
    ssize_t n;

    size_t nmatch = 4; // I designed it with 3 capture groups and whole string
    regmatch_t pmatch[nmatch];

    int accum=0;
    while( (n=getline(&line, &cap, fp))!= -1){ line[n-1] = '\0';
        regexec(&re, line, nmatch, pmatch, 0);

        char alphabet[26] = {0};
        int cpg_one_start   = (int)(pmatch[1].rm_so);
        int cpg_two_start   = (int)(pmatch[2].rm_so);
        int cpg_three_start = (int)(pmatch[3].rm_so);

        int cpg_one_len   = (int)(pmatch[1].rm_eo - cpg_one_start);
        int cpg_two_len   = (int)(pmatch[2].rm_eo - cpg_two_start);
        int cpg_three_len = (int)(pmatch[3].rm_eo - cpg_three_start);

        int id = atoi(&line[cpg_two_start]);

        for(int i=0; i< cpg_one_len; i++){
            char c=line[cpg_one_start + i];
            if (c=='-') continue;
            printf("%c", c);
            alphabet[c-'a'] += 1;
        }
        printf("\t ID: %i\n", id);

        for(int i=0; i<26; i++){
            printf(" %c ", i + 'a');
        }
        printf("\n");

        for(int i=0; i<26; i++){
            printf(" %i ", alphabet[i]);
        }

        printf("\n");
        // printf("Full String < %s >\n", &line[pmatch[0].rm_so]);
        // printf("First capture group < %.*s >\n", cpg_one_len, &line[cpg_one_start]);
        // printf("Second capture group < %.*s >\n", cpg_two_len, &line[cpg_two_start]);
        printf("Third capture group < %.*s >\n", cpg_three_len, &line[cpg_three_start]);

        char real_checksum[6]={0};
        int highest_idx = 0;

        for(int i=0; i<5; i++){
            int curr_val = 0;
            for(int ii=0; ii<26; ii++){
                if (alphabet[ii] > curr_val){
                    curr_val = alphabet[ii];
                    highest_idx = ii;
                }
            }
            alphabet[highest_idx] = 0;
            real_checksum[i] = 'a' + highest_idx;
        }

        printf("Real checkcum: %s\n", real_checksum);

        int real=1;
        for(int i=0; i<5; i++){
            if (real_checksum[i] != line[cpg_three_start + i]){
                real=0; break;
            }
        }
        if (real) accum += id;
    }

    regfree(&re);
    free(line);
    return accum;
}
