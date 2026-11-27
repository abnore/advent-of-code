#include "main.h"
#include <blackbox.h>
#include <stdint.h>

static void printarr(int *arr, int len)
{
    for(int i=0; i<len; i++){
        int x =arr[i];
        if (x == -1){
            printf(".");
        } else {
            printf("%d", x);
        }
    }
    printf("\n");
}

// Takes pointers to two values in the array, swaps the values
#define swap(ptr_a, ptr_b) do { \
    int _temp = *ptr_a; *ptr_a = *ptr_b; *ptr_b = _temp; \
} while(0)

int pt1(FILE* fp){
    // Read in the whole line
    int8_t buf[50000];
    int c, buf_count=0;
    while((c=fgetc(fp))!=EOF){
        buf[buf_count++] = (int8_t)(c-'0');
    }
    buf[ --buf_count ] = 0;
    // verify reading every char
    // printf("%i\n", buf_count);
    // int p=buf_count, num=0; while(p--) printf("%i",buf[num++]);
    // printf("\n");

    // create the array with ids and . for empty, using -1. inc id only when -1

    int massive_buf[1024*1024] = {0};
    int buf_index=0;
    int empty=-1;
    int id=0;
    int8_t type=1;
    for(int i=0; i<buf_count; ++i){
        int num =(int) buf[i];
        // printf("%i", num);

        while(num--){
            massive_buf[buf_index++] = type ? id : empty;
        }
        type ^= 1;
        if(type) id++;
    }
    // printf("\n");

    // printarr(massive_buf, buf_index);

    // swap backwards, filling in forwards. Two pointers, one move
    int *p_start = &massive_buf[0];
    int *p_end   = &massive_buf[buf_index-1];

    int count=0;

    for(int i=0; i<buf_index;i++){
        count=i;
        int *p_local = p_start + i;
        int curr = *p_local;

        if( p_local >= p_end) break;
        if (curr != -1) continue;

        swap(p_local, p_end);
        while(*p_end == -1) p_end--;

        // printarr(massive_buf, buf_index);
    }
    count++;

    // Now we have fragmented its time to sum all the ids
    uint64_t accum=0;
    for(int i=0; i<count; i++){
        int curr = massive_buf[i];
        int factor = curr * i;
        accum += factor;
    }
    printf("accum %llu\n", accum);

    return accum;
}
