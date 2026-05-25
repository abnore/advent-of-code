#include "main.h"
#include <blackbox.h>

static int all_factors(int c) {
    int sum=0;
    // printf("Factors of %d are: ", c);
    for (int i = 1; i * i <= c; i++) {
        if (c % i == 0) {
            // printf("%d ", i);
            sum += i*10;
            if (i * i != c) {
                sum += (c / i) * 10;
                // printf("%d ", c / i);
            }
        }
    }
    return sum;
}

int pt1(){
    int answer=1;
    int num = 33100000;

    int total;
    int i=1;
    for(; i<10000000; i++){
        total = all_factors(i);
        if (total >= num) break; // at least as many!
    }

    return i;
}
