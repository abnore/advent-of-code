#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    // 1. Must start with "test" or "input"
    if (strncmp(path, "test", 4) != 0 &&
            strncmp(path, "input", 5) != 0) {
        ERROR("Filename must start with 'test' or 'input'");
        return -1;
    }

    // 2. File must actually exist
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
