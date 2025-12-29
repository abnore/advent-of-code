#include <blackbox.h>
#include "main.h"

int main(int argc, char** argv)
{
    (void)argc, (void) argv;
    init_log(LOG_DEFAULT);

    const char* str = "bgvyzdsv";

    int num = pt1(str);
    INFO("part 1 is %d", num);
    num = pt2(str);
    INFO("part 2 is %d", num);

    shutdown_log();
    return(0);
}
