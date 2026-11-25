#include <blackbox.h>
#include <stdint.h>
#include "main.h"

int main(int argc, char** argv)
{
    init_log(LOG_DEFAULT);
    if(argc < 2) {
        ERROR("Insert path name");
        return -1;
    }
    char *path = argv[1];

    uint64_t answer=0;

    answer = pt1();
    if (answer) INFO("Part 1: %llu", answer);

    answer = pt2();
    if (answer) INFO("Part 2: %llu", answer);

    shutdown_log();
    return 0;
}
