#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/errno.h>

typedef struct {
    char *name;
    int value;
    char *instructions[3];
    int count;
}wire;

/* Globals */
wire *w_list;
int w_count=0;

typedef struct{
   const char* instruction;
   int result;
} cache_entry;

// cache for memoization - 1024 should be enough
#define CACHE_SIZE 1024
static cache_entry c[CACHE_SIZE]={0};
static int cache_count=0;

static void cache_insert(const char* instruction, int result){
    DEBUG("Caching \"%s\": %i", instruction, result);
    c[cache_count].instruction=strdup(instruction);
    c[cache_count++].result=result;
}
static int cache_lookup(char *instruction) {
    for(int i=0;i<cache_count;++i){
        if(!strcmp(c[i].instruction, instruction)){
            TRACE("is cached \"%s\":%i", instruction, c[i].result);
            return (int)c[i].result;
        }
    }
    return -1;
}

/* Read in every line and deconstruct the wires and instructions */
static int parse_file(FILE *fp, wire *w_list, int *w_count){
    char* line=NULL;
    size_t cap=0;
    ssize_t n;

    char tmp[256]={0};

    while((n=getline(&line, &cap, fp))>0) {
        line[n-1]='\0';

        strcpy(tmp, line);
        char *t;
        wire *w = &w_list[(*w_count)++];

        for(t=strtok(tmp, " "); t; t=strtok(NULL, " ")) {
            //when token is "->" we stop looping, to save the last
            if(!strcmp(t, "->")) break;
            w->instructions[w->count++] = strdup(t);
        }
        // right hand side - wire
        w->name = strdup(strtok(NULL, " "));
    }
    free(line);

    return n;
}
static wire *find_wire(const char *name) {
    for(int i=0;i<w_count; ++i)
        if(!strcmp(w_list[i].name, name))
            return &w_list[i];
    return NULL;
}
/* Inline command function */
static inline uint16_t NOT(uint16_t num){
    return (uint16_t)(~num);
}
static inline uint16_t AND(uint16_t val1, uint16_t val2){
    return (uint16_t)(val1 & val2);
}
static inline uint16_t OR(uint16_t val1, uint16_t val2){
    return (uint16_t)(val1 | val2);
}
static inline uint16_t RSHIFT(uint16_t val, int x){
    return (uint16_t)(val >> x);
}
static inline uint16_t LSHIFT(uint16_t val, int x){
    return (uint16_t)(val << x);
}
static int make_num(char *num) {
    return (uint16_t)strtol(num, (char **)NULL, 10);
}
static int is_num(char *possible_num) {
    int len = strlen(possible_num);
    if(len==1) {
        char ch = possible_num[0] - '0';
        return ch <= 9 && ch >= 0;
    }
    return atoi(possible_num);
}
// i know its max 3 instructions - no fancy logic here
static int create_instruction(wire *a, char *string){
    int n=0;
    n = sprintf(string, "%s", a->instructions[0]);
    if(a->count>1)
        n += sprintf(string, "%s %s",string, a->instructions[1]);
    if(a->count>2)
        n += sprintf(string, "%s %s",string, a->instructions[2]);
    return n;
}
// Recursive function to find a value
static uint16_t extract_value(wire *a){
    uint16_t result=-1;

    /* If the result is cached, return it */
    char _instruction[64]={0};
    if(create_instruction(a, _instruction)<0)
        ERROR("i dunno..");

    int cached = cache_lookup(_instruction);
    if(cached!=-1) {
        return (uint16_t)cached;
    } else {
        TRACE("\"%s\" Not cached", _instruction);
    }

    DEBUG("extracting value of \"%s\"->\"%s\"", a->name, _instruction);
    /* Break condition is a value */
    if(a->count == 1) {
        DEBUG("%s has count 1", a->name);
        if(is_num(a->instructions[0])){
            a->value = make_num(a->instructions[0]);
        }
        else {
            WARN("Not a number, wire!");
            wire *b = find_wire(a->instructions[0]);
            a->value = extract_value(b);
        }
        DEBUG("returning value %i", a->value);
        return a->value;
    }

    /* If not cached, or at the end of recursions
     * we parse through the instructions here
     * */

    wire *op_1 = NULL;
    wire *op_2 = NULL;

    /* 2 is always NOT */
    if(a->count == 2){
        op_1 = find_wire(a->instructions[1]);
        uint16_t value = extract_value(op_1);
        result = NOT(value);
    }

    /* Need to resolve which command */
    if(a->count == 3){
        char *cmd = a->instructions[1];
        uint16_t value_l;
        uint16_t value_r;

        if(is_num(a->instructions[0])){
            value_l = make_num(a->instructions[0]);
        } else {
            op_1 = find_wire(a->instructions[0]);
            value_l = extract_value(op_1);
        }
        if(is_num(a->instructions[2])){
            value_r = make_num(a->instructions[2]);
        } else {
            op_2 = find_wire(a->instructions[2]);
            value_r = extract_value(op_2);
        }
        /* Evaluate the commands */
        if(!strcmp(cmd, "AND")){
            result = AND(value_l, value_r);
        } else
        if(!strcmp(cmd, "OR")){
            result = OR(value_l, value_r);
        } else
        if(!strcmp(cmd, "LSHIFT")){
            result = LSHIFT(value_l, value_r);
        } else
        if(!strcmp(cmd, "RSHIFT")){
            result = RSHIFT(value_l, value_r);
        }
    }

    /* At the end of this function, if we get here we cache the result */
    cache_insert(_instruction, result);
    a->value = result;

    TRACE("Recursion over - returning \"%s\" with value %i ", a->name, a->value);
    return result;
}

int pt1(FILE* fp){
    int answer=0;
    w_list = calloc(500, sizeof *w_list);

    /* Parsing file creating the list of wires with respective instructions */
    parse_file(fp, w_list, &w_count);

    wire *w = find_wire("a");
    if(w){

        uint16_t v = extract_value(w);
        INFO("value of %s is %i", w->name, v);
    }
    INFO("Value of \"%s\" is %i", w->name, w->value);
    free(w_list);
    return answer;
}
