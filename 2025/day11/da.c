#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
/* Tsoding's implementation of a dynamic array in C with my own string builders */

typedef struct{
    int *items;
    size_t count;
    size_t capacity;
} Numbers;

typedef struct{
    const char **items;
    size_t count;
    size_t capacity;
} Names;

define da_append(xs, x) \
     do {\
        if(xs.count >= xs.capacity){\
            if (xs.capacity == 0) xs.capacity = 256;\
            xs.capacity <<= 1;\
            xs.items = realloc(xs.items, xs.capacity*sizeof(*xs.items));\
        }\
        xs.items[xs.count++] = x;\
    }while(0)\

#define da_free(xs) \
    do { \
        free((xs).items); \
        (xs).items = NULL; \
        (xs).count = (xs).capacity = 0; \
    } while (0)

/* My own string builders */
char *copy_str(const char *s)
{
    size_t n=0;
    while(s[n++]!=0); // manual strlen
    char *p = malloc(n);
    if (p) {
        while(n--) p[n]=s[n]; // manual memcpy
    }
    return p;
}

static char *str_fmt(const char *fmt, ...)
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

int main(void)
{
    Numbers xs = {0};
    Names ns = {0};

    for (int x=0;x<10;++x){
//        if(xs.count >= xs.capacity){
//            if (xs.capacity == 0) xs.capacity = 256;
//            xs.capacity <<= 1;
//            xs.items = realloc(xs.items, xs.capacity*sizeof(*xs.items));
//        }
//        xs.items[xs.count++] = x;
        da_append(xs, x);
        da_append(ns, str_fmt("Hi %d", x));
    }
    
    for(int i=0; i<xs.count; i++) printf("%i\n", xs.items[i]);
    for(int i=0; i<xs.count; i++) printf("%s\n", ns.items[i]);

    return 0;
}
