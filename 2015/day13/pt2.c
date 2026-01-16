#include "main.h"
#include "hash.h"
#include <blackbox.h>
#include <string.h>

#define match(s,n) strcmp(s,n)==0
#define strip(str) do{\
        int __l=strlen(str);\
        if(str[__l-1]=='\n') str[__l-1]=0;\
        if(str[__l-2]=='.')  str[__l-2]=0;\
        }while(0)

static hashmap indices={0};
static const char *names[9]={0};
static int name_count=0;
static int happiness[9][9];
static int seen[9]={0};

static int highest=0;

static void rec_perm(int *seen, int idx, int level, int curr_sum){
    seen[idx]=1;
    if (level == name_count-1){
        curr_sum += happiness[idx][0] + happiness[0][idx];

        if (curr_sum > highest){
            highest = curr_sum;
        }
        goto done;
    }
    for(int i=1; i<name_count; ++i){
        if(seen[i]) continue;

        int pair = happiness[idx][i] + happiness[i][idx];
        rec_perm(seen, i, level+1, pair+curr_sum);
    }
done:
    seen[idx]=0;
}


int pt2(FILE* fp){
    int answer=0;
    char *line=NULL;
    size_t cap=0;
    ssize_t n;

    char first_name[16];

    while((n=getline(&line, &cap, fp))>0){
        strip(line);
        sscanf(line, "%s", first_name);

        if ( hm_contains_key(&indices, first_name))
                continue;

        hm_put(&indices, first_name, name_count);
        names[name_count] = strdup(first_name);
        name_count++;
    }

    /* Insert myself into the list */
    hm_put(&indices, "me", name_count);
    names[name_count]=strdup("me");
    name_count++;

    rewind(fp);
    while((n=getline(&line, &cap, fp))>0){
        strip(line);

        char last_name[16];
        char g_or_l[4];
        int value;

        sscanf(line, "%s would %s %i happiness units by sitting next to %s.\n",
                first_name, g_or_l, &value, last_name);

        int sign = match(g_or_l, "lose")? -1 : 1;
        int s_val = sign*value;
        int i1 = hm_get(&indices, first_name);
        int i2 = hm_get(&indices, last_name);

        happiness[i1][i2] = s_val;
        TRACE("%s and %s : %d", first_name, last_name,s_val);
    }



    for(int i=0; i<name_count; ++i){
        happiness[hm_get(&indices, "me")][i] = 0;
    }
    for(int i=0; i<name_count; ++i){
        happiness[i][hm_get(&indices, "me")] = 0;
    }

    for(int i = 0; i <name_count; ++i){
       printf("%s:%lu\n", names[i], hm_get(&indices, names[i]));
    }

    rec_perm(seen, 0, 0, 0);

    return highest;
}
