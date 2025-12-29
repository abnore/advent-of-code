#include "main.h"
#include <blackbox.h>

#define swap(T,a,b) do{T t=a; a=b; b=t;}while(0)

static inline void bubble(int *arr) {
    if (arr[0]>arr[1]) swap(int, arr[0], arr[1]);
    if (arr[1]>arr[2]) swap(int, arr[1], arr[2]);
    if (arr[0]>arr[1]) swap(int, arr[0], arr[1]);
}

int pt2(FILE* fp){
    int arr[3];
    int *l=&arr[0],*w=&arr[1],*h=&arr[2];

    char *line = NULL;
    size_t cap = 0;

    int total_length = 0;
    while(getline(&line, &cap, fp)>0){
        sscanf(line, "%ix%ix%i\n", l, w, h);
        int bow = (*l)*(*w)*(*h);

        INFO("bow is %i",bow);
        INFO("arr = [%i, %i, %i]", *l, *w, *h);
        bubble(arr);
        INFO("sorted arr = [%i, %i, %i]", *l, *w, *h);

        int ribbon = 2*(*l) + 2*(*w);
        INFO("Ribbon is %i", ribbon);

        INFO("Therefore we need bow+ribbon = %i", bow+ribbon);

        total_length += (bow+ribbon);
    }
    INFO("total length of ribbon = %i", total_length);
    return(0);
}
