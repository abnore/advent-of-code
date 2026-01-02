#include "main.h"
#include <blackbox.h>
#include <string.h>

typedef enum{
    NORMAL,
    ESCAPE,
}state ;

static inline int is_num(char c){
    return (c-'0')<=9 && (c-'0')>=0;
}

static int parse(char *string, int len)
{
    int num=0;
    state s = NORMAL;
    for(int i=1; i<len-1; i++){
        char ch = string[i];
        TRACE("The ch: %c", ch);

        if (ch == '\\' && s == NORMAL){
            TRACE("state was normal, next escape");
            s = ESCAPE;
            continue;
        }

        switch (s) {
            case NORMAL:
                num++;
                TRACE("state is normal. Counting %i", num);
                break;
            case ESCAPE:
                TRACE("state is escape");
                switch (ch){
                    case '\\':
                        s = NORMAL;
                        num++;
                        TRACE("counting one char since \\ %i", num);
                        break;
                    case 'x':
                        s = NORMAL;
                        num++;
                        i+=2; // skip next 2 numbers
                        TRACE("counting one char num dont count after c %i", num);
                        break;
                    case '"':
                        s = NORMAL;
                        num++;
                        TRACE("counting one char \" %i", num);
                        break;
                }
                break;
        }
    }
    return num;
}
int pt1(FILE* fp)
{
    int answer=0;

    char *line=NULL;
    size_t cap=0;
    ssize_t n;
    while((n=getline(&line, &cap, fp))!=EOF){
        line[n-1]=0;
        DEBUG("%s",line);
        int len = strlen(line);
        TRACE("line is %i in memory", len);
        int p_len = parse(line, len);
        TRACE("Parsed line is %i", p_len);
        int diff = len-p_len;
        TRACE("Difference is %i", diff);
        answer += diff;
    }
    return answer;
}
