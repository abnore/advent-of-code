#include "main.h"
#include <blackbox.h>

//              speed       duration                           rest
// Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
// 1000 seconds later

static int calculate_speed(int time, reindeer r){
    int leg=0, time_leg=0, rem=time;

    time_leg += r.duration + r.rest;
   
    while(rem>0){
        if(rem > r.duration)
            leg += r.speed * r.duration;

        DEBUG("First leg %d km, time %d - remainding %d", leg, time_leg, rem);
        if ( r.rest > rem)
            DEBUG("resting for %d - only %d left, done!", r.rest, rem);
        rem -= time_leg;
    }

    return leg;
}



int pt1(FILE* fp){
    int answer=1;

    char *line=NULL;
    size_t cap=0;

    int test = 1000;
    int real = 2503;

    int res;
    while(getline(&line, &cap, fp)>0){
        strip(line);
        reindeer r = {0};
        sscanf(line, "%15s can fly %d km/s for %d seconds,\
                but then must rest for %d seconds.",
                r.name, &r.speed, &r.duration, &r.rest);

        DEBUG("%s - %d - %d - %d", r.name, r.speed, r.duration, r.rest);

        res = calculate_speed(real, r);
        if(res > answer) answer = res;
    }
    return answer;
}
