#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
/*

    There are certain patterns we needed to understand to solve this puzzle.
    I have spent many days now on this one, it was absolutely the hardest. And
    I refused to believe Eric wanted people to solve RREF Gaussian
    eliminations, as that is to advanced. I saw a pattern which worked for the
    test cases but came up short on the real input. That was finding unique
    bits and then maxing out those buttons and then sorting the numbers from
    high to low, and adding up the max amount without overshooting (basicly to
    the lowest amount allowed on the bits that were "hit"). That didnt work, i
    undershot by a slight amount, meaning apart from edge cases, it was almost
    the correct algorightm => Greedy-Naive. 
    Then i saw the connection with part 1, and looked into the logic of solving
    for a goal end state. Realized that a button hit once is lights on, but
    twice (or every even time) will result in a lights off state; Or stated
    more clearly: Pressing a button twice is not special => it just adds +2
    everywhere that button touches.

    I then created a goal pattern, and solve for the pattern like part 1, I
    then created the patterns and memoization technique. 

    First decide which buttons are pressed odd amount of times.
    Then divide the remaining problem by 2.

    Repeat until nothing is left.”
*/

/* Hard limits – puzzle input is small */
#define MAX_BITS    16 // I think i only have 10 max, but added some for safety
#define MAX_BUTTONS 16 // not this many but whatever
#define MAX_PATTERNS 1024
#define INF 1000000000

typedef struct {
    int vec[MAX_BITS];  // how much each counter increases
    int size;           // how many buttons were pressed
} pattern;

/*
    Patterns are grouped by parity mask.
    parity mask = bit i set if vec[i] is odd.
    If vec(x) does not match the parity of goal,
    then (goal - vec(x)) / 2 is impossible.

    You cannot cleanly divide odd numbers by 2!
    So only patterns whose parity matches the goal are legal.

    This single rule kills 99% of invalid branches

        bit i = 1 if v[i] is odd
    This matters because:
    If goal[i] is odd, the pattern must be odd at i, or (goal - vec)/2 breaks.
    No parity match means impossible branch.

    This is the entire pruning rule.
*/
static pattern patterns[1 << MAX_BITS][MAX_PATTERNS];
static int pattern_count[1 << MAX_BITS];

/* Simple memo cache for recursion */
typedef pattern cache_entry;
static cache_entry cache[200000];
static int cache_len = 0;

static int num_bits;

/* Check if goal is fully satisfied */
static int goal_is_zero(const int *g)
{
    for (int i = 0; i < num_bits; ++i)
        if (g[i] != 0)
            return 0;
    return 1;
}

/* Compute parity mask of a vector with some bit fiddling */
static int parity_mask(const int *v)
{
    int mask = 0;
    for (int i = 0; i < num_bits; ++i)
        if (v[i] & 1)
            mask |= (1 << i);
    return mask;
}

/* Check v <= goal */
static int vec_fits(const int *v, const int *goal)
{
    for (int i = 0; i < num_bits; ++i)
        if (v[i] > goal[i])
            return 0;
    return 1;
}

/* (goal - v) / 2 */
static void reduce_goal(int *dst, const int *goal, const int *v)
{
    for (int i = 0; i < num_bits; ++i)
        dst[i] = (goal[i] - v[i]) >> 1;
}

/* cache lookup for my makeshift cache for memoization, linear is fine */
static int cache_lookup(const int *goal)
{
    for (int i = 0; i < cache_len; ++i) {
        if (memcmp(cache[i].vec, goal, sizeof(int)*num_bits) == 0)
            return cache[i].size;
    }
    return -1;
}

static void cache_store(const int *goal, int value)
{
    memcpy(cache[cache_len].vec, goal, sizeof(int)*num_bits);
    cache[cache_len].size = value;
    cache_len++;
}

/* Recursion to solve the patterns and cache results - making this manageable */
static int solve_rec(const int *goal)
{
    if (goal_is_zero(goal))
        return 0;

    int cached = cache_lookup(goal);
    //DEBUG("%0x%x, cached", cached);
    if (cached >= 0)
        return cached;

    int best = INF;
    int pm = parity_mask(goal);
    TRACE("0x%x -> pm ", pm);
    /*
        Only patterns with matching parity are legal.
        This is the whole trick.
    */
    for (int i = 0; i < pattern_count[pm]; ++i) {
        pattern *p = &patterns[pm][i];

        if (!vec_fits(p->vec, goal))
            continue;

        int next[MAX_BITS];
        reduce_goal(next, goal, p->vec);

        int sub = solve_rec(next);
        if (sub < INF) {
            int candidate = p->size + 2 * sub;
            DEBUG("%d candidate", candidate);
            if (candidate < best)
                best = candidate;
        }
    }

    /* And we store it for easier lookup later */
    cache_store(goal, best);
    return best;
}


static void build_patterns(int coeffs[MAX_BUTTONS][MAX_BITS], int button_count)
{
    memset(pattern_count, 0, sizeof(pattern_count));

    int subsets = 1 << button_count;

    /*
        Enumerate ALL subsets of buttons.
        This is exactly Part 1.
    */
    for (int mask = 0; mask < subsets; ++mask) {
        int vec[MAX_BITS] = {0};
        int size = 0;

        for (int b = 0; b < button_count; ++b) {
            if (mask & (1 << b)) {
                size++;
                for (int i = 0; i < num_bits; ++i)
                    vec[i] += coeffs[b][i]; // add instead of xor
            }
        }

        int pm = parity_mask(vec);
        int idx = pattern_count[pm]++;

        memcpy(patterns[pm][idx].vec, vec, sizeof(int)*num_bits);
        patterns[pm][idx].size = size;
    }
}

static int parse_line(char *line)
{
    int coeffs[MAX_BUTTONS][MAX_BITS]; // The buttons are coefficients and bits
    int button_count = 0;
    int goal[MAX_BITS]; // The end state

    /* Count number of bits from first input, but ignore it */
    num_bits = 0;
    for (char *p = line; *p && *p != ']'; ++p)
        if (*p == '.' || *p == '#')
            num_bits++;

    /* Parse button definitions */
    char *p = line;
    while ((p = strchr(p, '('))) {
        p++;
        memset(coeffs[button_count], 0, sizeof(int)*num_bits);

        while (*p && *p != ')') {
            if (*p >= '0' && *p <= '9') {
                int idx = *p - '0';
                coeffs[button_count][idx] = 1;
            }
            p++;
        }
        button_count++;
    }

    /* Parse target joltage */
    p = strchr(line, '{') + 1;
    for (int i = 0; i < num_bits; ++i) {
        goal[i] = atoi(p);
        while (*p && *p != ',' && *p != '}') p++;
        if (*p == ',') p++;
    }

    cache_len = 0;
    build_patterns(coeffs, button_count);

    return solve_rec(goal);
}

int pt2(FILE *fp)
{
    char *line = NULL;
    size_t cap = 0;
    ssize_t n;

    uint64_t total = 0;

    while ((n = getline(&line, &cap, fp)) > 0) {
        if (line[n-1] == '\n')
            line[n-1] = '\0';

        total += parse_line(line);
    }

    INFO("total presses is %lld", total);
    free(line);
    return 0;
}
