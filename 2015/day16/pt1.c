#include "main.h"
#include <blackbox.h>
#include <string.h>

int pt1(FILE* fp){
    char *line = NULL;
    size_t cap = 0;
    ssize_t n;
    int sue_num=0;
    hashmap hm= {0};
    hm_put(&hm, "children",3 );
    hm_put(&hm, "cats",7 );
    hm_put(&hm, "samoyeds",2 );
    hm_put(&hm, "pomeranians",3 );
    hm_put(&hm, "akitas",0 );
    hm_put(&hm, "vizslas",0 );
    hm_put(&hm, "goldfish",5 );
    hm_put(&hm, "trees",3 );
    hm_put(&hm, "cars",2 );
    hm_put(&hm, "perfumes",1 );

    while( (n=getline(&line, &cap, fp)) > 0 )
    { if( line[n] == '\n') line[n] = 0;

        char tmp[256];
        strcpy(tmp, line);
        char *t = strtok(tmp, " ");
        t = strtok(NULL, ":");
        sue_num = atoi(t);
        int miss=0;

        for (t = strtok(NULL, ",:"); t; t = strtok(NULL, ",:"))
        { while(*t == ' ') t++;
            int value = hm_get(&hm, t);
            t = strtok(NULL, ",:");
            if (atoi(t) != value){
                miss = 1;
                break;
            }
        }
        if (miss == 0) break;
    }
    return sue_num;
}
