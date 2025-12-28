#include "main.h"
#include <blackbox.h>

int pt2(FILE* fp){
    
    char c;
    int t=0;
    int pos=0;
    while((c=fgetc(fp))>0){
        if (c=='('){
            t++;
        } else
        if (c==')'){
            t--;
        }
        pos++;

        if(t<0) break;
    }
    INFO("t is %i, pos is %i", t, pos);
    return 0;
}

