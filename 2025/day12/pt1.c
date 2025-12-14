#include "main.h"
#include <blackbox.h>
#include <string.h>
#include <stdlib.h>

#define NUM_SHAPES 6
 
/* I will borrow a trick from researching how chess engines work.
 * I will count the number of black and white squares occupied, and that
 * must be less then the total, we can throw it out if it is 4x4 is 8 black,
 * 8 white - and the example shape takes up 4 black and 3 white. 2 of then CAN fit, 
 * if the orientation works - This means we can massivly reduce the search area
 * Instead of just counting squares, chess pattern will give a parity, and the same amount
 * of B&W ratio..... 
 *
 *
 * nevermind.. i re-read the problem - We are just to compare areas.. 
 * Almost every region is massively under-filled, or the packages sum up to more area, 
 * making it impossible
 *
 * Therefore the code we need to write is just to compare the sum of both area and region
 * */

static inline int cell_color(int x, int y) {
    return (x + y) & 1;
}

typedef struct {
    int idx;
    char cells[3][4];  // 3 rows of "###\0"
    int area;
} Shape;

Shape shapes[NUM_SHAPES] = {0}; // keeping things reeeally simple with a global 

typedef struct {
    int row, col;
    int idx[6];
    int w,b;
}Region;

static void print_shape(Shape s)
{
    DEBUG("Printing shape %d", s.idx);
    for (int r = 0; r < 3; r++)
        DEBUG("  %s", shapes[s.idx].cells[r]);

}
static void print_region(Region r)
{
    INFO("Printing region %dx%d", r.col, r.row);
    for(int row=0;row<r.row;row++)
    {
        for(int col=0;col<r.col;col++)
        {
            if(cell_color(row, col)) printf("b ");
            else printf("w ");
        }
        printf("\n");
    }

}
static int read_shapes(FILE *fp)
{
    char *line = NULL;
    size_t cap = 0;

    for (int s = 0; s < NUM_SHAPES; s++) {

        /* read "i:" line, skipping blanks */
        int index = -1;
        while (getline(&line, &cap, fp) > 0) {
            if (line[0] == '\n') continue;
            if (sscanf(line, "%d:", &index) == 1)
                break;
        }

        shapes[index].idx = index;
        shapes[index].area = 0;  // init to 0

        /* read 3 rows */
        for (int r = 0; r < 3; r++) {
            getline(&line, &cap, fp);
            line[strcspn(line, "\n")] = 0;  // strip newline
            strncpy(shapes[index].cells[r], line, 3);
            shapes[index].cells[r][3] = 0;
        }

        /* count number of '#' */
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                if (shapes[index].cells[r][c] == '#')
                    shapes[index].area++;
            }
        }

        print_shape(shapes[index]);
        DEBUG("Shape %d area = %d", index, shapes[index].area);
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
    DEBUG("Read region: %s", line);
    free(line);
    return ret;
}
// Well this was a waste of time... 
//static void rotate_shape(Shape *s)
//{
//    char tmp[3][4]; // 3 rows, "###\0"
//
//    for (int y = 0; y < 3; y++) {
//        for (int x = 0; x < 3; x++) {
//            tmp[y][x] = s->cells[2 - x][y];
//        }
//        tmp[y][3] = '\0';
//    }
//
//    memcpy(s->cells, tmp, sizeof(tmp));
//}


int pt1(FILE *fp)
{
    Region reg = {0};
    read_shapes(fp);
//    rotate_shape(&shapes[0]);
//    print_shape(shapes[0]);

    int valid_regions = 0;

    int res = 0;
    while (read_region(fp, &reg) > 0) {

        int region_area = reg.col * reg.row;
        res += region_area;

        int area_used = 0;
        for (int i = 0; i < 6; ++i) {
            area_used += reg.idx[i] * shapes[i].area;
        }
        INFO("Area used by packages is %i", area_used);
        INFO("Area of the region is %i", region_area);
        if (region_area >= area_used) {
            valid_regions++;
        }
    }

    INFO("total area in regions is %i", res);
    INFO("regions that can fit shapes = %d", valid_regions);
    return 0;
}
