#include "main.h"
#include <blackbox.h>

#define test(x) ( x[0]+x[1] > x[2] && x[0]+x[2] > x[1] && x[1]+x[2] > x[0] )

int pt2(FILE* fp){
    int answer=0;

    int a[3],b[3],c[3];

    char *line=NULL;
    size_t cap=0;

    int linenum=0;
    while( (getline(&line, &cap,  fp)) != -1) {
        sscanf(line, "%d %d %d\n", &a[linenum], &b[linenum], &c[linenum]);

        if (linenum == 2){ // read three times
            if( test(a) ) {
                answer++;
            }
            if( test(b) ) {
                answer++;
            }
            if( test(c) ) {
                answer++;
            }
        }
        linenum++;
        linenum %= 3;
    };
    free(line);

    return answer;
}
