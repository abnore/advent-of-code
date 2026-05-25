#include "main.h"
#include <blackbox.h>
#include <string.h>

typedef struct{
    size_t hashes[1024];
    int len;
} Hash;

// borrow my own hash function, to hash the strings. Low chance of collisions
// on a 64 bit hash, easier to check if they are the same.
static size_t hash_string(const char* key) {
    size_t hash = 0xcbf29ce484222325;
    for (const char* s = key; *s; s++){
        hash ^= (size_t)(unsigned char)(*s);
        hash *= 0x100000001b3;
    }
    return hash;
}

static void replace(const char *input, char *new_str,
                    char *org_mol, char *rep_mol,
                    int idx )
{
    int input_len = strlen(input);
    int input_mol_len = strlen(org_mol);
    int mol_replen = strlen(rep_mol);

    int ptr=0;
    for(int i=0; i<input_len; i++){
        if (i==idx){
            for(int k=0;k<mol_replen;k++){
                new_str[ptr++] = rep_mol[k];
            }
            i+=(input_mol_len-1);
        } else {
            new_str[ptr++] = input[i];
        }
    }
    new_str[ptr] = '\0';
}
// const char* input = "HOHOHO";
static const char* input = "ORnPBPMgArCaCaCaSiThCaCaSiThCaCaPBSiRnFArRnFArCaCaSiThCaCaSiThCaCaCaCaCaCaSiRnFYFArSiRnMgArCaSiRnPTiTiBFYPBFArSiRnCaSiRnTiRnFArSiAlArPTiBPTiRnCaSiAlArCaPTiTiBPMgYFArPTiRnFArSiRnCaCaFArRnCaFArCaSiRnSiRnMgArFYCaSiRnMgArCaCaSiThPRnFArPBCaSiRnMgArCaCaSiThCaSiRnTiMgArFArSiThSiThCaCaSiRnMgArCaCaSiRnFArTiBPTiRnCaSiAlArCaPTiRnFArPBPBCaCaSiThCaPBSiThPRnFArSiThCaSiThCaSiThCaPTiBSiRnFYFArCaCaPRnFArPBCaCaPBSiRnTiRnFArCaPRnFArSiRnCaCaCaSiThCaRnCaFArYCaSiRnFArBCaCaCaSiThFArPBFArCaSiRnFArRnCaCaCaFArSiRnFArTiRnPMgArF";

static int insert_hash(Hash *h, size_t hash){

    if (h->len >= 1024) return 1;
    for(int i=0; i<h->len; i++){
        if (h->hashes[i] == hash) return 1;
    }
    h->hashes[h->len++] = hash;
    return 0;
}

/*
 * OH
 * HOHOHO
 *   ^
 */
int find_mol(const char* input, char* org, int idx){
    int inputlen=strlen(input);
    int len = strlen(org);

    int pos;
    for(pos=idx; pos<inputlen; pos++){
        if(input[pos] == org[0]){
            for(int i=0; i<len; i++){
                if( input[pos+i] == org[i] ) continue;
                else goto next;
            }
            return pos+1;
        }
next:
        continue;
    }

    return -1;
}

/* HOH has 5, but only 4 distinct, HOHOHO has 9, but only 7 distinct */
int pt1(FILE* fp){

    char tstr[1024]={0};
    Hash hashes={0};

    char *line = NULL;
    size_t cap=0;
    ssize_t n=0;

    char org[10]={0};
    char rep[10]={0};
    while ( (n=getline(&line, &cap, fp)) != EOF ){
        sscanf(line, "%s => %s\n", org, rep);
        int ret=0;
        while( (ret=find_mol(input, org, ret)) != -1) {

            replace(input, tstr, org, rep, ret-1);
            size_t h = hash_string(tstr);

            insert_hash(&hashes, h);

        }
    }

    return hashes.len;
}
