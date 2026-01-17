#include "main.h"
#include <blackbox.h>

static int calculate_speed(int time, int num, reindeer *r)
{
    /* Set this do debug and look at logic*/
    //time = 1000;

    int sec = 0;
    int highest = 0;

    while (sec < time) {

        for (int i = 0; i < num; ++i) {

            TRACE("%s is %s with %d left",
                  r[i].name,
                  r[i].resting ? "resting" : "moving",
                  r[i].resting ? r[i].countdown : r[i].moving);

            if (!r[i].resting) {
                r[i].distance += r[i].speed;
                if (--r[i].moving == 0) {
                    r[i].resting = 1;
                    r[i].countdown = r[i].rest;
                }
            } else {
                if (--r[i].countdown == 0) {
                    r[i].resting = 0;
                    r[i].moving = r[i].duration;
                }
            }

            if (r[i].distance > highest)
                highest = r[i].distance;

            TRACE("%s got %d in sec %d",
                   r[i].name, r[i].distance, sec);
        }

        for (int i = 0; i < num; ++i) {
            if (r[i].distance == highest)
                r[i].points += 1;

            TRACE("\t%s has %d points",
                   r[i].name, r[i].points);
        }

        sec++;
    }

    int max_points=0;
    for (int i = 0; i < num; ++i) {
        if(r[i].points > max_points) max_points = r[i].points;
    }

    return max_points;
}

int pt2(FILE* fp){
    int answer=1;

    char *line=NULL;
    size_t cap=0;

    int test = 1000;
    int real = 2503;

    int res;

    int herd_num=0;
    reindeer r_herd[9]={0}; // max 9- but num will override for test
    int herd_index=0;

    while(getline(&line, &cap, fp)>0){
        strip(line);
        reindeer *r = &r_herd[herd_index++];
        sscanf(line, "%15s can fly %d km/s for %d seconds,\
                but then must rest for %d seconds.",
                r->name, &r->speed, &r->duration, &r->rest);

        DEBUG("%s - %d - %d - %d", r->name, r->speed, r->duration, r->rest);
        herd_num++;
        r->countdown = r->rest;
        r->moving = r->duration;

    }
    answer = calculate_speed(real, herd_num, r_herd);

    for(int i=0; i<herd_num;++i){
        TRACE("%s has %d points\n", r_herd[i].name, r_herd[i].points);
    }
    return answer;
}
