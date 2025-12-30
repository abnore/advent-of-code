#include "main.h"
#include <blackbox.h>
#include <string.h>

// minimum of 3 - which means we can optimize and stop counting
// at 3. Returns the num, less than 3 means naughty
static int min_3vowels(char *line)
{
    char c; int idx=0;
    char vowels[5] = {'a','e','i','o','u'};
    int v_count=0;
    while((c=line[idx++]) != '\0') {
       for(int i=0; i<5; ++i)
           if(c==vowels[i]) v_count++;

       if(v_count >= 3) break;
    }
    return v_count;
}
// Twice in a row makes this very easy, not overflowing - reading 0 byte
static int dup_letter(char *line)
{
    char c,d; int idx=0;
    while((c=line[idx++]) != '\0')
        if(c==(d=line[idx])) return 1;
    return(0); // found none
}
// shall not contain ab, cd, pq, xy
static int contains(char *line)
{
    if(!strcmp(line,"")) return -1;
    if(strstr(line, "ab")) return 1;
    if(strstr(line, "cd")) return 1;
    if(strstr(line, "pq")) return 1;
    if(strstr(line, "xy")) return 1;
    return 0;
}

int pt1(FILE* fp){
    (void)fp;
    int answer=0;

    char *line = NULL;
    size_t linecap=0;
    ssize_t len;
    while((len=getline(&line, &linecap, fp))!=EOF)
    {
        if(line[len-1]=='\n') line[len-1]=0;
        int min = min_3vowels(line);
        int dup = dup_letter(line);
        int con = contains(line);

        if(min>=3 && dup && !con) answer++;
    };
    return answer;
}
