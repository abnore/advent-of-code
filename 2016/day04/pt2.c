#include "main.h"
#include <blackbox.h>
#include <regex.h>

static char rotate(char a, int num){
    if (a=='-') return ' ';
    int _a = (a - 'a') + num ;
    return  ('a' + (_a % 26));
}

int pt2(FILE* fp){
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
        int cpg_two_start   = (int)(pmatch[2].rm_so);
        int cpg_three_start = (int)(pmatch[3].rm_so);

        int cpg_one_len   = (int)(pmatch[1].rm_eo);
        int cpg_two_len   = (int)(pmatch[2].rm_eo - cpg_two_start);
        int cpg_three_len = (int)(pmatch[3].rm_eo - cpg_three_start);

        int id = atoi(&line[cpg_two_start]);

        for(int i=0; i< cpg_one_len; i++){
            char c=line[i];
            if (c=='-') continue;
            alphabet[c-'a'] += 1;
        }

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
        // printf("Real checkcum: %s\n", real_checksum);
        int real=1;
        for(int i=0; i<5; i++){
            if (real_checksum[i] != line[cpg_three_start + i]){
                real=0; break;
            }
        }
        if (real) {

            char decrypted[cpg_one_len + 1];
            decrypted[cpg_one_len] = '\0';

            for(int i=0; i<cpg_one_len; ++i){
                decrypted[i] = rotate(line[i], id);
            }

            printf("ID: %d - %s\n",id, decrypted);
        }
    }

    regfree(&re);
    free(line);
    return 0;
}
