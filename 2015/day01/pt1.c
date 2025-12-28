#include "main.h"
#include <blackbox.h>

int pt1(FILE* fp){
    
    char c;
    int t=0;
    while((c=fgetc(fp))>0){
        if (c=='('){
            t++;
        } else
        if (c==')'){
            t--;
        }
    }
    INFO("t is %i", t);
    return 0;
}
