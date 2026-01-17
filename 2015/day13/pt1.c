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
#define first_level_limit(level, n) ((level) == 0 ? ((n) + 1) / 2 : (n))

static inline int perm(int n){
    int __n=n;
    while(--__n){
        n *=__n;
    }
    return n;
}

int rec_perm(int *leaf_count, int name_count, int *seen,
             int idx, int level, int happiness[][8])
{
    seen[idx]=1;
    int pair;

    if (level == name_count-1){
        (*leaf_count)++;
        seen[idx]=0;
        pair  = happiness[idx][0] + happiness[0][idx];
        DEBUG("LINE %d - idx %d, level %d - %d",*leaf_count, idx, level, pair);
        return pair;
    }

    int best_branch=0;

    // being a cycle, i know the (n-1) is mirrored, and we only have to do half
    int limit = first_level_limit(level, name_count);

    for(int i=1; i<limit; ++i){
        if(seen[i]) continue;
        pair = happiness[idx][i] + happiness[i][idx];

        int temp = pair + rec_perm(leaf_count, name_count, seen, i, level+1, happiness);
        DEBUG("idx %d, level %d - %d", idx, level, pair);

        if (temp > best_branch) best_branch = temp;
    }
    seen[idx]=0;

    return best_branch;
}

int pt1(FILE* fp)
{
    char *line=NULL;
    size_t cap=0;

    char first_name[16];
    int name_count=0;
    int happiness[8][8];
    const char *names[8];

    hashmap indices={0};

    while(getline(&line, &cap, fp)>0){
        strip(line);
        sscanf(line, "%s", first_name);

        if (hm_contains_key(&indices, first_name))
                continue;

        hm_put(&indices, first_name, name_count);
        names[name_count] = strdup(first_name);
        name_count++;
    }
    rewind(fp);

    while(getline(&line, &cap, fp)>0){
        strip(line);

        char last_name[16];
        char gain_or_lose[4];
        int value;

        sscanf(line, "%s would %s %i happiness units by sitting next to %s.\n",
                first_name, gain_or_lose, &value, last_name);

        int s_val = value * (*gain_or_lose=='l'? -1 : 1);
        int i1 = hm_get(&indices, first_name);
        int i2 = hm_get(&indices, last_name);

        happiness[i1][i2] = s_val;
        TRACE("%s and %s : %d", first_name, last_name,s_val);
    }


    int seen[8]={1};
    int permutations = perm(name_count);
    int max = permutations>>1;
    int curr_line=0;

    return rec_perm(&curr_line, name_count, seen, 0, 0, happiness);
}
