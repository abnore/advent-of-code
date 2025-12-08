#include <blackbox.h>
#include "main.h"
#include <math.h>

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

    pt1(fp);
    fseek(fp, 0, SEEK_SET);
    pt2(fp);

    fclose(fp);
    shutdown_log();
    return 0;
}
