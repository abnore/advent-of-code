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
    JMP,
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
    uchar ret = offset & 0x3f;
    if(sign) ret |= 0x20;

    printf("making offset %i from %i with sign %c\n", ret, offset, sign?'-':'+');
    return ret;
}
static char decode_offset(uchar offset){
    uchar sign = offset >> 5;
    uchar num = offset & 0x1f;
    char ret;

    if(sign) ret = -num;
    else     ret = num;

    // printf("decoding offset %i from %i\n", ret, offset);
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
        printf("offset is %i, opcode %#x\n", offset, opcode);
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
        printf("offset is %i, opcode %#x\n", offset, opcode);
    } else
    if(!strcmp(opcode_str, "jio")){
        opcode = 0x80 | (JIO<<6) | offset;
        printf("offset is %i, opcode %#x\n", offset, opcode);
    }
    binary[pc++] = opcode;
}


static void block0(uchar op){
    if (op & 0x40) {
        uchar offset = op & 0x3f;
        // printf("jmp, %#x\n", offset);
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

    // printf("block1: op %#x -> opcode %i, reg %c, offset %i\n",op, opcode, reg_idx?'b':'a', offset);
    switch(opcode){
    case JIE:
        if ((reg % 2) == 0){
            pc += decode_offset(offset) - 1;
            // printf("Jumping because register is even\n");
        }
        break;

    case JIO:
        if (reg == 1) {
            // printf("Jumping because register is 1\n");
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

int pt1(FILE* fp)
{
    FILE *bin = fopen("bin", "wb");
    char *line = NULL;
    size_t cap = 0;
    ssize_t n = 0;

    registers[0]=0; // a
    registers[1]=0; // b

    while( (n=getline(&line, &cap, fp))!=EOF )
    { if(line[n-1]=='\n') line[n-1]='\0';
        // printf("%s", line);
        extract_opcode(line, n);
    }

    for(int i=0; i<pc; i++)
    {
        (void)fwrite(&binary[i],sizeof(uchar), 1, bin);
        // if ((i % 16)==0 && i>0) printf("\n");
        // printf("%.2x",(uchar)binary[i]);
    }
    // printf("\n");
    fclose(bin);
    int binary_length = pc;
    pc = 0;

    while(pc < binary_length){
        // printf("\t\tPC: %i\n", pc);
        decode(binary[pc]);
        pc++;
    }

    return registers[1];
}
