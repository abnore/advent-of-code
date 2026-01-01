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

    int answer;

    answer = pt1(fp);
    if(answer)
        INFO("Part 1: %d", answer);

    fseek(fp, 0, SEEK_SET);

    answer = pt2(fp);
    if(answer)
        INFO("Part 2: %d", answer);

    fclose(fp);
    shutdown_log();
    return 0;
}
