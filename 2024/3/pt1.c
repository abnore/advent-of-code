#include "main.h"
#include <string.h>
#include <stdio.h>
#include <blackbox.h>


#define BUF 50000

int pt1(FILE *fp)
{
    char bigbuf[BUF];
    size_t n = fread(bigbuf, 1, BUF - 1, fp);
    bigbuf[n] = '\0';

    int sum = 0;
    char *p = bigbuf;

    while ((p = strstr(p, "mul(")) != NULL) {
        int a, b;
        int n = -1;
        if (sscanf(p, "mul(%d,%d)%n", &a, &b, &n) == 2 && n != -1) {
            sum += a * b;
        }

        p++;  // move forward to avoid infinite loop
    }

    INFO("Sum = %d", sum);
    return sum;
}
