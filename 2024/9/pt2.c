#include "main.h"
#include <blackbox.h>
#include <stdint.h>


// Scans from left up to start of group requesting. Returns index of spot free,
// or -1 of nothing found
static int scan_for_space(int*arr, int len, int *start){
    int count=0;
    for(int i=0; &arr[i] != start; i++)
    {
        if(arr[i] == -1) count ++;
        else count=0;

        if (count == len) {
            return i;
        }
    }
    return -1;
}

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

int pt2(FILE* fp){
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

    // Now we have to insert a check, if the file (group of end_idS can fit)
    // I will try counting ahead and backwards while we go up. Leave as much of
    // the old impl as I can
    // Attempt to move the file excactly once! in order words, the Id has to be
    // desending
    int prev_id=INT32_MAX;
    for(int i=0; i<buf_index;i--){

        // Counting backwards for file end_id size until we get a match
        int end_id_count=0;
        int *checkpoint = p_end;
        int end_id = *p_end;
        if(end_id == 0) break;
        if(end_id == -1 || end_id >= prev_id) {
            p_end--;
            continue;
        }

        prev_id = end_id;
        while(*checkpoint == end_id) {
            checkpoint--;
            end_id_count++;
        }

        // Counting empty spots forward until we fit, or we continue!
        int available = scan_for_space(massive_buf, end_id_count, checkpoint+1);
        if (available < 0) {
            p_end -= end_id_count;
            continue;
        }
        // printf("End end_id count %i -  End id: %d - ", end_id_count, end_id);
        // printf("Spaces: %i - ", available);


        for(int i=0; i<end_id_count; i++){
            swap( &massive_buf[available], p_end  );
            available--; p_end--;
        }
        // printarr(massive_buf, buf_index);
    }

    // Now we have fragmented its time to sum all the ids
    uint64_t accum=0;
    for(int i=0; i<buf_index; i++){
        uint64_t curr = massive_buf[i];

        // printf("%lli - ", curr);
        if(curr == -1) {
            continue;
        }

        uint64_t factor = curr * i;
        accum += factor;
    }
    printf("accum %llu\n", accum);

    return accum;
}
