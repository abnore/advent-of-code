#include "main.h"
#include <blackbox.h>
#include <string.h>

// duplicate with on letter sandwiched - may overflow
static int dup_letter(char *line)
{
    char c,d; int idx=0;
    while((c=line[idx++]) != '\0'){
        if(line[idx]==0) return 0; // safely check bounded
        if(c==(d=line[idx+1])) return 1;
    }
    return(0); // found none
}
// shall contain
static int contains(char *line)
{
    if(!strcmp(line,"")) return -1;

    int len = strlen(line);
    /* construct a string based on 2 letters */
    char test[3]={0};
    char new[3]={0};
    char c;

    for(int i=0; i<len-1; i++)
    {
        test[0] = line[i];
        test[1] = line[i+1];
        for(int ii=i+2; ii<len-1; ii++){
            new[0] = line[ii];
            new[1] = line[ii+1];
            if(!strcmp(new,test)){
                return 1;
            }
        }
    }
    return 0;
}

int pt2(FILE* fp){
    (void)fp;
    int answer=0;

    char *line = NULL;
    size_t linecap=0;
    ssize_t len;

    while((len=getline(&line, &linecap, fp))!=EOF)
    {
        if(line[len-1]=='\n') line[len-1]=0;
        int dup = dup_letter(line);
        int con = contains(line);

        if(dup && con) answer++;

    };
    return answer;
}
