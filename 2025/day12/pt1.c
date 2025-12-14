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
 * of B&W ratio
 * */

typedef struct {
    char cells[3][4];  // 3 rows of "###\0"
} Shape;

Shape shapes[NUM_SHAPES] = {0}; // keeping things reeeally simple with a global 

int read_shapes(FILE *fp)
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
        INFO("Reading shape %d", index);

        /* read 3 rows */
        for (int r = 0; r < 3; r++) {
            getline(&line, &cap, fp);
            line[strcspn(line, "\n")] = 0;  // strip newline
            strncpy(shapes[index].cells[r], line, 3);
            shapes[index].cells[r][3] = 0;
        }

        DEBUG("Shape %d:", index);
        for (int r = 0; r < 3; r++)
            DEBUG("  %s", shapes[index].cells[r]);
    }
    free(line);
    return 0;
}

static inline int cell_color(int x, int y)
{
    return (x + y) & 1;
}
int pt1(FILE *fp)
{
    read_shapes(fp);
    return 0;
}
