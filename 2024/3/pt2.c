#include "main.h"
#include <blackbox.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUF 50000

static int parse_number(const char *s, int *out, int *consumed)
{
    int val = 0;
    int i = 0;

    if (!isdigit((unsigned char)s[i]))
        return 0;

    while (isdigit((unsigned char)s[i])) {
        val = val * 10 + (s[i] - '0');
        i++;
    }

    *out = val;
    *consumed = i;
    return 1;
}

int pt2(FILE *fp)
{
    char buf[BUF];
    size_t len = fread(buf, 1, BUF - 1, fp);
    buf[len] = '\0';

    int enabled = 1;
    long sum = 0;

    for (size_t i = 0; i < len; ) {

        if (strncmp(&buf[i], "do()", 4) == 0) {
            enabled = 1;
            i += 4;
            continue;
        }

        if (strncmp(&buf[i], "don't()", 7) == 0) {
            enabled = 0;
            i += 7;
            continue;
        }

        /* mul(X,Y) */
        if (strncmp(&buf[i], "mul(", 4) == 0) {
            size_t j = i + 4;
            int a, b, n;

            /* parse X */
            if (!parse_number(&buf[j], &a, &n))
                goto advance;
            j += n;

            if (buf[j] != ',')
                goto advance;
            j++;

            /* parse Y */
            if (!parse_number(&buf[j], &b, &n))
                goto advance;
            j += n;

            if (buf[j] != ')')
                goto advance;
            j++;

            if (enabled)
                sum += (long)a * b;

            i = j;
            continue;
        }

advance:
        i++;
    }

    INFO("Result: %ld", sum);
    return 0;
}
