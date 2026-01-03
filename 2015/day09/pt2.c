#include "main.h"
#include <blackbox.h>
#include <string.h>

#define PATHS 32

static char *cities[8];
static int c_num=0;

static int p_idx=0;

static struct path {
    char from[16];
    char to[16];
    int distance;
} p_list[PATHS];
typedef struct path path;

/* Read in every object and store it*/
static void parse_file(FILE *fp){
    char *line=NULL;
    size_t cap=0;
    while(getline(&line, &cap, fp)!=EOF) {
        path *p = &p_list[p_idx++];
        sscanf(line, "%15s to %15s = %d", p->from, p->to, &p->distance);
        TRACE("creating path %i", p_idx);

        /* Store the unique cities */ 
        if(c_num==0){
            cities[c_num++] = p->from;
            cities[c_num++] = p->to;
        } else if(!strcmp(cities[0], p->from)){
            cities[c_num++] = p->to;
        }
    }
}

static int dist(const char* a, const char *b){
    for(int i=0; i<p_idx; ++i){
        if((!strcmp(p_list[i].to, a) && !strcmp(p_list[i].from, b)) ||
                (!strcmp(p_list[i].to, b) && !strcmp(p_list[i].from, a))
          )
            return p_list[i].distance;
    }
    return -1; //impossible
}
static void dfs(int last, int depth, int cur_dist, int *used, int *best)
{
    if(depth == c_num){
        if(cur_dist > *best) // reversed logic
            *best = cur_dist;
        return;
    }

    for(int i=0;i<c_num;i++){
        if(used[i]) continue;

        int d = 0;
        if(last != -1){
            d = dist(cities[last], cities[i]);
        }
            

        used[i] = 1;
        dfs(i, depth+1, cur_dist + d, used, best);
        used[i] = 0;
    }
}


int pt2(FILE* fp){
    parse_file(fp);

    int used[8] = {0};
    int best = 0; // just reverse the logic

    dfs(-1, 0, 0, used, &best);

    return best;
}
