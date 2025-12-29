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

    int res;

    res = pt1(fp);
    INFO("Part 1 answer: %d", res);

    fseek(fp, 0, SEEK_SET);

    res = pt2(fp);
    INFO("Part 2 answer: %d", res);

    fclose(fp);
    shutdown_log();
    return 0;
}
