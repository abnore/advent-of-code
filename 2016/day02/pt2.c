#include "main.h"
#include <blackbox.h>
/*
        1
      2 3 4
    5 6 7 8 9
      A B C
        D
*/
#define up(a)       do{ \
    if( a!=5 && a!=2 && a!=1 &&a!=4 && a!=9) { \
        if(a==13 || a==3) a-=2;  else a-=4;}\
}while(0)

#define down(a)     do{ \
    if( a!=5 && a!=10 && a!=13 &&a!=12 && a!=9) { \
        if(a==11 || a==1) a+=2;  else a+=4;}\
}while(0)

#define left(a)     do{ \
    if( a!= 1 && a!=2 && a!=5 && a!=10 && a!=13) a-=1; \
}while(0)

#define right(a)     do{ \
    if( a!= 1 && a!=4 && a!=9 && a!=12 && a!=13) a+=1; \
}while(0)

int pt2(FILE* fp)
{
    int start = 5;
    int pin[10]={0};
    int pin_count=0;

    char *line = NULL;
    size_t cap = 0;
    ssize_t n;

    while ((n = getline(&line, &cap, fp)) > 0) {
        for(int i=0; i<n-1; i++){
            switch(line[i]){
                case 'U': up(start);    break;
                case 'D': down(start);  break;
                case 'R': right(start); break;
                case 'L': left(start);  break;
                default:
                    printf("Something has gone wrong here\n");
                    break;
            }
        }
        printf("%X", start);
    }
    printf("\n");

    free(line);

    return 2;
}
