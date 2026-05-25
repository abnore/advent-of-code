#include <blackbox.h>
#include "main.h"

int main(int argc, char** argv)
{
    init_log(LOG_DEFAULT);

    int answer;

    answer = pt1();
    if (answer) INFO("Part 1: %d", answer);

    answer = pt2();
    if (answer) INFO("Part 2: %d", answer);

    shutdown_log();
    return 0;
}
