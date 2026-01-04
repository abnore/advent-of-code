#include "main.h"
#include <blackbox.h>
#include <string.h>
#include <stdlib.h>

static int parse_value(char **p);
static int parse_object(char **p);
static int parse_array(char **p);

static int is_digit(char c){
    return ( (c-'0') >= 0 && (c-'0')<=9 );

}
/* Gets the value, and steps past it so 16 doesnt become 6 after one step */
static int parse_number(char **p) {
    int val = atoi(*p);
    while (**p == '-' || is_digit(**p)) 
        (*p)++;
    return val;
}

/* Parse a string, return 1 if it is exactly "red" */
static int parse_string_is_red(char **p) {
    int is_red = 0;

    (*p)++; /* skip opening quote */

    if ((*p)[0] == 'r' && (*p)[1] == 'e' && (*p)[2] == 'd' && (*p)[3] == '"')
        is_red = 1;

    while (**p && **p != '"')
        (*p)++;

    if (**p == '"')
        (*p)++; /* Skips closing qoute */

    return is_red;
}

static int parse_object(char **p) {
    int sum = 0;
    int red = 0;

    (*p)++; /* skip '{' */

    while (**p && **p != '}') {
        if (**p == '"') {
            parse_string_is_red(p);
            if (**p == ':')
                (*p)++;
            if (**p == '"') {
                if (parse_string_is_red(p))
                    red = 1;
            } else {
                int v = parse_value(p);
                sum += v;
            }
        } else {
            (*p)++;
        }
    }

    if (**p == '}')
        (*p)++;

    return red ? 0 : sum;
}

static int parse_value(char **p) {
    if (**p == '{')
        return parse_object(p);
    if (**p == '[')
        return parse_array(p);
    if (**p == '"' ) {
        parse_string_is_red(p);
        return 0;
    }
    if (**p == '-' || is_digit(**p))
        return parse_number(p);

    (*p)++;
    return 0;
}


/* Parse an array - "red" has no effect */
static int parse_array(char **p) {
    int sum = 0;

    (*p)++; /* skip '[' */

    while (**p && **p != ']') {
        sum += parse_value(p);
    }

    if (**p == ']')
        (*p)++;

    return sum;
}

int pt2(FILE* fp) {
    char *line = NULL;
    size_t cap = 0;

    if (getline(&line, &cap, fp) <= 0)
        return 0;

    char *p = line;
    int answer = parse_value(&p);

    free(line);
    return answer;
}
