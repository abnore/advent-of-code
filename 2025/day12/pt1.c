#include "main.h"
#include <blackbox.h>
#include <string.h>
#include <stdlib.h>

#define NUM_SHAPES 5

/* I will borrow a trick from researching how chess engines work.
 * I will count the number of black and white squares occupied, and that
 * must be less then the total, we can throw it out if it is 4x4 is 8 black,
 * 8 white - and the example shape takes up 4 black and 3 white. 2 of then CAN fit, 
 * if the orientation works - This means we can massivly reduce the search area
 * Instead of just counting squares, chess pattern will give a parity, and the same amount
 * of B&W ratio
 * */

typedef struct {
    int idx;
    char cells[3][4];  // 3 rows of "###\0"
} Shape;
Shape shapes[NUM_SHAPES] = {0}; // keeping things reeeally simple with a global 

typedef struct {
    int row, col;
    int idx[6];
}Region;


static void print_shape(Shape s)
{
        INFO("Reading shape %d", s.idx);
        DEBUG("Shape %d:", s.idx);
        for (int r = 0; r < 3; r++)
            DEBUG("  %s", shapes[s.idx].cells[r]);

}
int read_shapes(FILE *fp)
{
    char *line = NULL;
    size_t cap = 0;
    for (int s = 0; s <= NUM_SHAPES; s++) {

        /* read "i:" line, skipping blanks */
        int index = -1;
        while (getline(&line, &cap, fp) > 0) {
            if (line[0] == '\n') continue;
            if (sscanf(line, "%d:", &index) == 1)
                break;
        }
        shapes[index].idx = index;

        /* read 3 rows */
        for (int r = 0; r < 3; r++) {
            getline(&line, &cap, fp);
            line[strcspn(line, "\n")] = 0;  // strip newline
            strncpy(shapes[index].cells[r], line, 3);
            shapes[index].cells[r][3] = 0;
        }

        print_shape(shapes[index]);
    }
    free(line);
    return 0;
}
int read_region(FILE *fp, Region *r)
{
    char *line = NULL;
    size_t cap = 0;
    int ret = getline(&line, &cap, fp);
    if (line[0] == '\n') { // if newline, read it again
        TRACE("Read newline, reading in again");
        ret = getline(&line, &cap, fp);
    };

    line[strcspn(line, "\n")] = 0;  // strip newline
    sscanf(line, "%dx%d: %d %d %d %d %d %d",
            &r->col, 
            &r->row,
            &r->idx[0],
            &r->idx[1],
            &r->idx[2],
            &r->idx[3],
            &r->idx[4],
            &r->idx[5]);
    INFO("Read region: %s", line);
    free(line);
    return ret;
}
static void rotate_shape(Shape *s)
{
    char tmp[3][4]; // 3 rows, "###\0"

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            tmp[y][x] = s->cells[2 - x][y];
        }
        tmp[y][3] = '\0';
    }

    memcpy(s->cells, tmp, sizeof(tmp));
}


static inline int cell_color(int x, int y)
{
    return (x + y) & 1;
}
int pt1(FILE *fp)
{
    Region reg = {0};
    read_shapes(fp);
    rotate_shape(&shapes[0]);
    print_shape(shapes[0]);

    int ret = read_region(fp, &reg);
    INFO("%i return value for regions", ret);
    ret = read_region(fp, &reg);
    INFO("%i return value for regions", ret);
    ret = read_region(fp, &reg);
    INFO("%i return value for regions", ret);
    ret = read_region(fp, &reg);
    INFO("%i return value for regions", ret);
    return 0;
}
