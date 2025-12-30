#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
   int x1,x2,y1,y2;
   int toggle;
   int lights;
   int through;
} State;

void parse_line(char *line, State *s) {
    char tmp[256];
    strcpy(tmp, line);

    for (char *t = strtok(tmp, " "); t; t = strtok(NULL, " ")) {
        TRACE("Token is %s",t);
        if(!strcmp(t, "toggle")) {
            s->toggle=1;
            s->lights=0;
            continue;
        }
        if(!strcmp(t, "turn")){
            s->toggle=0;
            continue;
        }
        if(!strcmp(t, "on")){
            s->lights=1;
            continue;
        }
        if(!strcmp(t, "off")){
            s->lights=0;
            continue;
        }
        if(!strcmp(t, "through")){
            s->through=1;
            continue;
        }

        if(s->through) {
            sscanf(t, "%i,%i", &s->x2, &s->y2);
            continue;
        } else {
            sscanf(t, "%i,%i", &s->x1, &s->y1);
        }
    }
}
#define ROW 1000
#define COL 1000

int pt1(FILE* fp){
    int answer=0;

    int grid[COL][ROW]={0};
    char *line=NULL;
    size_t cap=0;
    ssize_t n;

    int count = 0;
    while( (n = getline(&line, &cap, fp)) !=EOF )
    {
        State s = {0};
        line[n-1]=0;

        DEBUG("Line is: \"%s\"", line);
        parse_line(line, &s);

        for(int col=s.y1; col<=s.y2; col++){
            for(int row=s.x1; row<=s.x2; row++){

                if(s.toggle){
                    DEBUG("Toggling");
                    grid[col][row] ^= 1;
                } else {
                    DEBUG("Turning %s",s.lights?"on":"off" );
                    grid[col][row] = s.lights;
                }
                DEBUG("grid[%i][%i] = %d", col, row, grid[col][row]);
            }
        }

        DEBUG("toggle=%s lights=%s x1=%i, x2=%i, y1=%i, y2=%i",
                s.toggle?"on":"off", s.lights?"on":"off", s.x1, s.x2, s.y1, s.y2 );
    }

    /* Final Tally */
    for(int y=0; y<COL; y++){
        for(int x=0; x<ROW; x++){
            if(grid[y][x]==1) count++;
        }
    }

    answer = count;
    return answer;
}
