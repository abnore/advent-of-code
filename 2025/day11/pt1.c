#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* My own string helper */
char *copy_str(const char *s)
{
    size_t n=0;
    while(s[n++]!=0); // manual strlen
    char *p = malloc(n);
    if (p) {
        while(n--) p[n]=s[n]; // manual memcpy
    }
    return p;
}

/* 
   Directed Acyclid Graph (DAG)
*/

typedef struct node {
    const char *name;
    struct node **out;   // outgoing edges
    size_t out_count;
    size_t out_cap;
} node;

/* Dynamic array of strings - using Tsoding macro da_append() */
typedef struct {
    node **items;
    size_t count;
    size_t capacity;
} node_table;

/* Simple getter  which returns a node if it exist, otherwise creates it */
static node *get_node(node_table *t, const char *name)
{
    for (size_t i = 0; i < t->count; i++) {
        if (strcmp(t->items[i]->name, name) == 0)
            return t->items[i];
    }

    DEBUG("Creating %s", name);
    node *n = calloc(1, sizeof(*n));
    n->name = copy_str(name);

    da_append((*t), n);

    return n;
}

static void add_edge(node *from, node *to)
{
    if (from->out_count == from->out_cap) 
    {
        from->out_cap = from->out_cap ? from->out_cap << 1 : 4;
        from->out = realloc(from->out, from->out_cap * sizeof(*from->out));
    }
    from->out[from->out_count++] = to;
    TRACE("Adding edge from %s to %s", from->name, to->name);
}

static int count_paths(node *n)
{

    /* Exit requirement for breaking recursion */
    if (strcmp(n->name, "out") == 0)
    {
        TRACE("Found out");
        return 1;
    }

    /* Just walk the graph and count, return 1 when reaching out
     * Recursively go down, DFS, and pop up with the total 1 at the bottom */
    int total = 0;
    for (size_t i = 0; i < n->out_count; i++) {
        DEBUG("counting paths from %s with out_count: %zu", n->name, n->out_count);
        total += count_paths(n->out[i]);
    }

    return total;
}

int pt1(FILE *fp)
{
    node_table table = {0};

    char *line = NULL;
    size_t cap = 0;

    while (getline(&line, &cap, fp) > 0) {
        /* Remove new line from each line */
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        char *tmp = copy_str(line);

        /* Read in left side of line, and get/create node */
        char *left_s = strtok(tmp, ":");
        node *src = get_node(&table, left_s);

        /* For each device on right side, get or create node and add as edge */
        for (char *tok = strtok(NULL, " "); tok; tok = strtok(NULL, " ")) {
            node *dst = get_node(&table, tok);
            add_edge(src, dst);
        }
        /* Whatever */
        free(tmp);
    }

    node *start = get_node(&table, "you");
    int result = count_paths(start);

    INFO("total paths = %d", result);
    
    da_free(table);
    free(line);
    return 0;
}
