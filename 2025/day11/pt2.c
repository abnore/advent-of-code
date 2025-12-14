#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>

/* 
   pt2 is cyclical! Need backtacking with cycles
*/

typedef struct node {
    const char *name;

    struct node **out;
    size_t out_count, out_cap;

    struct node **in;
    size_t in_count, in_cap;

    int visited;

    int can_reach_out;
    int can_reach_dac;
    int can_reach_fft;

    /* Needed memoization here */
    long long memo[2][2];
    int memo_valid[2][2];
} node;

/* Dynamic array of strings - using Tsoding macro da_append() */
typedef struct {
    node **items;
    size_t count;
    size_t capacity;
} node_table;

/* My own string helper */
static char *copy_str(const char *s)
{
    size_t n = 0;
    while (s[n++] != 0);
    char *p = malloc(n);
    if (p) while (n--) p[n] = s[n];
    return p;
}

static int is_name(node *n, const char *s)
{
    return strcmp(n->name, s) == 0;
}

/* Simple getter */
static node *get_node(node_table *t, const char *name)
{
    for (size_t i = 0; i < t->count; i++)
        if (strcmp(t->items[i]->name, name) == 0)
            return t->items[i];

    DEBUG("Creating node %s", name);
    node *n = calloc(1, sizeof(*n));
    n->name = copy_str(name);
    
    da_append((*t), n);

    return n;
}

static void add_edge(node *from, node *to)
{
    TRACE("Adding edge from %s to %s", from->name, to->name);
    if (from->out_count == from->out_cap) {
        from->out_cap = from->out_cap ? from->out_cap << 1 : 4;
        from->out = realloc(from->out, from->out_cap * sizeof(*from->out));
    }
    from->out[from->out_count++] = to;

    /* reverse edge */
    if (to->in_count == to->in_cap) {
        to->in_cap = to->in_cap ? to->in_cap << 1 : 4;
        to->in = realloc(to->in, to->in_cap * sizeof(*to->in));
    }
    to->in[to->in_count++] = from;
}

/* Reachability marking - We need a way to know if it can reach each before counting anything
 * We walk the graph backwards and mark which nodes are allowed to eventually
 * reach out, dac, or fft
 * By extension - every node that points to it can also reach out(or ffs/dac)!
 */

static void _can_reach_out(node *n)
{
    if (n->can_reach_out) return;
    n->can_reach_out = 1;
    for (size_t i = 0; i < n->in_count; i++)
        _can_reach_out(n->in[i]);
}

static void _can_reach_dac(node *n)
{
    if (n->can_reach_dac) return;
    n->can_reach_dac = 1;
    for (size_t i = 0; i < n->in_count; i++)
        _can_reach_dac(n->in[i]);
}

static void _can_reach_fft(node *n)
{
    if (n->can_reach_fft) return;
    n->can_reach_fft = 1;
    for (size_t i = 0; i < n->in_count; i++)
        _can_reach_fft(n->in[i]);
}

static void compute_reachability(node_table *t)
{
    for (size_t i = 0; i < t->count; i++) {
        node *n = t->items[i];
        
        if (is_name(n, "out"))
            _can_reach_out(n);
        if (is_name(n, "dac"))
            _can_reach_dac(n);
        if (is_name(n, "fft"))
            _can_reach_fft(n);
    }
}

/* DFS with memoization and hard pruning */

static long long count_paths(node *n, int seen_dac, int seen_fft)
{
    /* Prune all paths that cant reach out before DFS explodes */
    if (!n->can_reach_out)              return 0;
    if (!seen_dac && !n->can_reach_dac) return 0;
    if (!seen_fft && !n->can_reach_fft) return 0;

    if(n->memo_valid[seen_dac][seen_fft])
        return n->memo[seen_dac][seen_fft];

    if(n->visited)
        return 0;

    n->visited = 1;

    DEBUG("Visited: %s", n->name);
    int sd = seen_dac || is_name(n, "dac");
    int sf = seen_fft || is_name(n, "fft");

    long long total = 0;

    if (is_name(n, "out")) {
        total = (sd && sf);
    } else {
        for (size_t i = 0; i < n->out_count; i++)
            total += count_paths(n->out[i], sd, sf);
    }

    DEBUG("Finished with %s", n->name);
    /* preventing infinite looping - visited is path local, memo is global */
    n->visited = 0;

    n->memo_valid[seen_dac][seen_fft] = 1;
    n->memo[seen_dac][seen_fft] = total;

    return total;
}

int pt2(FILE *fp)
{
    node_table table = {0};
    char *line = NULL;
    size_t cap = 0;

    while (getline(&line, &cap, fp) > 0) {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        char *tmp = copy_str(line);
        node *src = get_node(&table, strtok(tmp, ":"));

        for (char *tok = strtok(NULL, " "); tok; tok = strtok(NULL, " "))
            add_edge(src, get_node(&table, tok));

        free(tmp);
    }

    compute_reachability(&table);

    long long result = count_paths(get_node(&table, "svr"), 0, 0);
    INFO("total paths = %lld", result);

    da_free(table);
    free(line);
    return 0;
}
