#include "main.h"
#include <blackbox.h>

int pt1(FILE* fp){
    int answer=0;
    int a,b,c;

    char *line=NULL;
    size_t cap=0;
    ssize_t n;

    while( (n=getline(&line, &cap,  fp)) != -1) {
        sscanf(line, "%d %d %d", &a, &b, &c);
        // 1. if 1+2 is bigger than 3
        if( a+b > c &&
        // 2. if 1+3 is bigger than 2
         a+c > b &&
        // 3. if 2+3 is bigger than 1
         b+c > a) {
            answer++;
            continue;
        }
        // printf("a=%i b=%i c=%i Not a triangle!\n", a, b, c);

    };
    free(line);

    return answer;
}
