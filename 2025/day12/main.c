#include <blackbox.h>
#include "main.h"

int main(int argc, char** argv)
{
    init_log(LOG_DEFAULT);
    if(argc < 2) {
        ERROR("Insert path name");
        return -1;
    }
    char *path = argv[1];

    FILE *fp = fopen(path, "r");
    if (!fp) {
        ERROR("File '%s' does not exist", path);
        return -1;
    }

    // day 12 has no part 2
    int ret = pt1(fp);

    INFO("Answer is %d", ret);
    fclose(fp);
    shutdown_log();
    return 0;
}
