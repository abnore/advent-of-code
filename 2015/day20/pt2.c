#include "main.h"
#include <blackbox.h>

static int all_factors(int c) {
    int sum=0;
    for (int i = 1; i * i <= c; i++) {
        if (c % i == 0) {
            int elf1 = i;
            int elf2 = c / i;
            if (c / elf1 <= 50)
                sum += elf1 * 11;
            if (elf2 != elf1 && c / elf2 <= 50)
                sum += elf2 * 11;
        }
    }
    return sum;
}

int pt2(){
    int answer=1;
    int num = 33100000;

    int total;
    int i=1;
    for(; i<1000000; i++){
        total = all_factors(i);
        if (total >= num) break; // at least as many!
    }

    return i;
}

