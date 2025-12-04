#include "main.h"
#include <blackbox.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int64_t is_repeated_pattern(int64_t num) {
    char s[32];
    sprintf(s, "%lld", num);
    int L = strlen(s);

    // Try all possible chunk sizes d
    for (int d = 1; d <= L/2; d++) {
        if (L % d != 0) continue;

        int repeats = L / d;
        int ok = 1;

        for (int r = 1; r < repeats; r++) {
            if (strncmp(s, s + r*d, d) != 0) {
                ok = 0;
                break;
            }
        }

        if (ok) {
            return num;
        }
    }

    return 0;
}

static int parse_ranges(char *line, range *ranges, int max) {
    int count = 0;

    char *token = strtok(line, ",");
    while (token && count < max) {
        int64_t a, b;
        if (sscanf(token, "%lld-%lld", &a, &b) == 2) {
            ranges[count].start = a;
            ranges[count].end = b;
            count++;
        }
        token = strtok(NULL, ",");
    }

    return count;
}

int64_t sum_invalid(range *ranges, int n) {
    int64_t sum = 0;

    for (int i = 0; i < n; i++) {
        int64_t a = ranges[i].start;
        int64_t b = ranges[i].end;

        DEBUG("Checking range %2d: %lld - %lld", i+1, a, b);

        for (int64_t x = a; x <= b; x++)
            sum += is_repeated_pattern(x);
    }

    return sum;
}

int pt2(FILE* fp)
{
    char buf[64000];
    fgets(buf, sizeof(buf), fp);

    range ranges[128];
    int n = parse_ranges(buf, ranges, 128);

    INFO("Parsed %d ranges", n);

    int64_t answer = sum_invalid(ranges, n);

    INFO("Final answer: %lld", answer);

    shutdown_log();
    return 0;
}
