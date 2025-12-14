#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* My own string builders */
static char *copy_str(const char *s)
{
    size_t n=0;
    while(s[n++]!=0); // manual strlen
    char *p = malloc(n);
    if (p) {
        while(n--) p[n]=s[n]; // manual memcpy
    }
    return p;
}

static const char *str_fmt(const char *fmt, ...)
{
    va_list args;

    // compute the length by writing to NULL, size 0
    va_start(args, fmt);
    int n = vsnprintf(NULL, 0, fmt, args);
    va_end(args); // must re init the va args list

    // Write to string s in format fmt variadics
    char *s = malloc(n + 1);
    va_start(args, fmt);
    vsnprintf(s, n + 1, fmt, args);
    va_end(args);

    return s;
}

typedef struct{
    const char **items;
    size_t count;
    size_t capacity;
} Devices ;

typedef struct{
    const char **items;
    size_t count;
    size_t capacity;
} Parents;

typedef struct{ 
    const char *name;
    size_t count;
    size_t capacity;
} Neighbours;

typedef struct{
    Parents *parents;
    Devices *children;
} Graph;

static int is_child(const char* parent, Devices *children){
    int count = children->count;
    if(count==0) return -1;

    for(int i=0; i < count; i++)
    {
        if(strcmp(parent, children->items[i])==0) return 1;
    }
    return 0;
}

int pt1(FILE* fp){
    
    Graph g = {0};

    /* Create a main parents and childrens list */
    *p = calloc(1,sizeof(Devices));
    Devices *c = calloc(1,sizeof(Devices));

//    g.parents = p;
//    g.children = c;

    char *line = NULL;
    size_t cap = 0;
    getline(&line, &cap, fp);
    char *tmp = copy_str(line);

    char *token=strtok(tmp,":");
    INFO("%s", token);

//    if(g.parents == NULL)
//    {
//        da_append((*p), token);
//    } else if(is_child(token, c)>0) {
//         
//    }
//
//    //da_append((*c), t);
//    if(is_child(token, c)<0) INFO("NO CHILDREN YET!");
//    if(is_child(token, c)>0) INFO(" MATCH!");
//    if(is_child(token, c)==0) INFO("NO MATCH!");
//
//    INFO("currently in parents: %s", p->items[0]);
//    INFO("g.parents->count=%zu", g.parents->count);

    for(token=strtok(NULL," ");token; token=strtok(NULL, " ")) {
        INFO("%s",token);
    }
    free(p);
    free(c);

    return 0;
}
