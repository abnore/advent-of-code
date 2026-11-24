#include "main.h"
#include <string.h>
#include <blackbox.h>

typedef unsigned int uint;
typedef unsigned char uchar;
// registers a=0 and b=1 as indexes into this array
static uint registers[2];

// create the binary
static uchar binary[1024]={0};
static int pc=0;

// opcodes
enum {
    // block 0
    HLF=0,
    TPL,
    INC,
    JMP=4,
    // block 1
    JIE=0,
    JIO,
};

// parse number
static uchar num(char *offset, int n){
    uchar res=0;
    for(int i=0;i<n;i++){
        res *= 10;
        res += (offset[i] - '0');
    }
    return res;
}

static uchar make_offset(uchar offset, int sign){
    uchar ret;

    if(sign)
        ret = 0x20 | (offset & 0x3f);
    else
        ret = offset & 0x3f;

    return ret;
}
static char decode_offset(uchar offset){
    uchar sign = offset >> 5;
    uchar num = offset & 0x1f;
    char ret;

    if (sign){
        ret = -num;
    } else
        ret = num;

    return ret;
}


// Created the binary based on the file being read, line by line
static void extract_opcode(char *line, int n){
    char opcode_str[4]={0};
    uchar opcode, offset, sign; // the byte for the opcode, including register
    int reg, i;

    for(i=0;i<3;i++){
        opcode_str[i] = line[i];
    }
    i++; // remove whitespace after opcode

    if(!strcmp(opcode_str, "jmp")){
        sign = line[i++]=='+'? 0 : 1;
        offset = make_offset(num(&line[i], n-i-1),sign);
        opcode = (JMP<<4) | offset;
        binary[pc++] = opcode;
        return;
    }
    // the rest has registers, some has offsets
    reg = line[i++]=='a'? 0 : 1;

    if(!strcmp(opcode_str, "hlf")){
        opcode = (HLF<<4) | reg;
        binary[pc++] = opcode;
        return;
    } else
    if(!strcmp(opcode_str, "tpl")){
        opcode = (TPL<<4) | reg;
        binary[pc++] = opcode;
        return;
    } else
    if(!strcmp(opcode_str, "inc")){
        opcode = (INC<<4) | reg;
        binary[pc++] = opcode;
        return;
    }

    // these have offsets
    i+=2; // remove comma and whitespace after register
    sign = line[i++]=='+'? 0 : 1;
    offset = make_offset(num(&line[i], n-i-1),sign);

    if(!strcmp(opcode_str, "jie")){
        opcode = 0x80 | (JIE<<6) | offset;
    } else
    if(!strcmp(opcode_str, "jio")){
        opcode = 0x80 | (JIO<<6) | offset;
    }
    binary[pc++] = opcode;
}


static void block0(uchar op){
    if (op & 0x40) {
        uchar offset = op & 0x3f;
        pc += decode_offset(offset) - 1;
        return;
    }

    uchar opcode = op >> 4;
    int reg_idx = op & 1;
    uint reg = registers[reg_idx];

    switch(opcode){
    case HLF:
        registers[reg_idx] /= 2;
        break;
    case TPL:
        registers[reg_idx] *= 3;
        break;
    case INC:
        registers[reg_idx] += 1;
        break;
    }
}

static void block1(uchar op){
    uchar opcode = (op >> 6) & 1;
    uint reg = registers[0];
    uchar offset = op & 0x3f;

    switch(opcode){
    case JIE:
        if ((reg % 2) == 0){
            pc += decode_offset(offset) - 1;
        }
        break;

    case JIO:
        if (reg == 1) {
            pc += decode_offset(offset) - 1;
        }
        break;
    }
}
static void decode(uchar op){
    uchar block = op & 0x80;

    if (block)  block1(op);
    else        block0(op);
}

int pt2(FILE* fp)
{
    char *line = NULL;
    size_t cap = 0;
    ssize_t n = 0;

    registers[0]=1; // a part 2 starts as 1 instead of 0
    registers[1]=0; // b

    while( (n=getline(&line, &cap, fp))!=EOF )
    { if(line[n-1]=='\n') line[n-1]='\0';
        extract_opcode(line, n);
    }

    int binary_length = pc;
    pc = 0;

    while(pc < binary_length){
        decode(binary[pc]);
        pc++;
    }

    return registers[1];
}
