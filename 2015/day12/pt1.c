#include "main.h"
#include <blackbox.h>
#include <string.h>

int pt1(FILE* fp){
    int answer=0;
    char *line=NULL;
    size_t cap=0;
    ssize_t n = getline(&line, &cap, fp);
    INFO("Read in 1 line, with %zi chars, %zu cap", n, cap);

    char *new = malloc(sizeof(char)*n);
    strncpy(new, line, n);
    INFO("copied %zi chars into new", n);

    char *t;
    for(t=strtok(new, "[:,"); t; t=strtok(NULL, "[:,")){

        DEBUG("%s",t);
        int num=atoi(t);
        if(num)
            answer+=num;
    }

    return answer;
}
