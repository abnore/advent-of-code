#include "main.h"
#include <blackbox.h>
#include <string.h>
#include <stdint.h>

static uint64_t best_qe=UINT64_MAX;
static uint64_t fewest_arr=UINT64_MAX;

static void sum_weight(int *weights, int len, int *store_arr, int level, int passing_along, int target)
{
    // printf("level: %i %i\n", level, passing_along);
    if(level == len) {
        int sum = 0;
        for(int i=0;i<len;i++) sum += store_arr[i];

        uint64_t quant_ent = 0;
        uint64_t arr_num_by_weights=0;

        if (sum == target) {
            // printf("sum %i - ", sum);

            uint64_t quant_ent = 1;

            for(int i=0;i<len;i++)
            {
                int val = store_arr[i];
                if(val) {
                    // printf("%i ", val) ;
                    arr_num_by_weights++;
                }
                val = val ? val: 1;
                quant_ent *= val;
            }
            // printf("\tquant_ent = %llu\n", quant_ent);
            if(arr_num_by_weights <= fewest_arr){
                fewest_arr = arr_num_by_weights;
                if(quant_ent < best_qe) best_qe = quant_ent;
            }
        }

        return;
    }

    // go left
    store_arr[level] = 0;
    sum_weight(weights, len, store_arr, level + 1, 0, target);
    // go right
    store_arr[level] = weights[level];
    sum_weight(weights, len, store_arr, level + 1, weights[level], target);
    return;
}

uint64_t pt2(FILE* fp)
{
    int weights[BUFSIZ] = {0};
    int pass_weights[BUFSIZ] = {0};
    char *line = NULL;
    size_t cap = 0;
    ssize_t n;
    int weight_num=0, sum=0;

    while ((n = getline(&line, &cap, fp)) > 0) {
        int t_num = atoi(line);
        weights[weight_num++] = t_num;
        sum += t_num;
    }
    free(line);

    int target = sum/4;
    printf("The sum of all numbers is %i, therefore each group is %i\n", sum, target);


    // Find groups of num/3 .. We need two pointers, one for current num, and
    // one for skipped
    // Outer loop for group one, one loop for group2 and last loop sums up what
    // is left. This is something that HAS to sum up, no remainder since we
    // know its a multiple of the same number

    // int test_len = 4;
    // int test_arr[] = { 1, 2, 3, 4 };
    // int pass_arr[4] = {0};
    // sum_weight(test_arr, test_len, pass_arr, 0, 0);

    sum_weight(weights, weight_num, pass_weights, 0, 0, target);
    // printf("%llu\n", best_qe);
    return best_qe;
}
