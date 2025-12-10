#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <stdint.h>

static inline uint64_t check_distance(Jbox j1, Jbox j2){
    int64_t a, b, c;
    a = (int64_t)j1.x-(int64_t)j2.x;
    b = (int64_t)j1.y-(int64_t)j2.y;
    c = (int64_t)j1.z-(int64_t)j2.z;
    return (uint64_t)((a*a) + (b*b) + (c*c));
}

static int cmp_pair(const void *a,const void *b) {
    const Pair *p1 = a, *p2 = b;
    if(p1->dist < p2->dist) return -1;
    if(p1->dist > p2->dist) return  1;
    return 0;
}

static int find(int *parent, int index){
    TRACE("parent[%i]=%i", index, parent[index]);
    if(parent[index] == index) 
        return index;
    return find(parent, parent[index]);
}

static void union_sets(int *parent, int *size, int a, int b) {
    a = find(parent, a);
    b = find(parent, b);
    if (a != b){
        if (size[a] < size[b]) {
            parent[a] = b;
            size[b] += size[a];
        } else {
            parent[b] = a;
            size[a] += size[b];
        }
    }
}

int pt2(FILE* fp){
    Jbox* boxes = NULL;
    size_t count = 0;
    size_t capacity = 0;

    char line[256];

    while (fgets(line, sizeof(line), fp)) {

        if (count == capacity) {
            capacity = capacity == 0 ? 16 : capacity << 1; 
            boxes = realloc(boxes, capacity * sizeof(Jbox));
        }

        uint64_t x, y, z;
        if (sscanf(line, "%llu,%llu,%llu", &x, &y, &z) == 3) {
            boxes[count].x = x;
            boxes[count].y = y;
            boxes[count].z = z;
            count++;
        }
    }
    int num_d = (count*(count-1))/2, num_pairs=0;

    Pair *pairs = malloc(num_d*sizeof(Pair));

    for (int i=0; i<count; ++i) {
        for(int ii=i+1; ii<count; ++ii){
            uint64_t dist = check_distance((boxes[i]), (boxes[ii]));
            Pair *p = &pairs[num_pairs++]; 
            p->dist = dist;
            p->ia = i;
            p->ib = ii;
        }
    }
    qsort(pairs, (size_t)num_pairs, sizeof(Pair), cmp_pair);

    int first_box[count]; 
    int size[count];     

    for(int i=0; i<count; ++i) {
        first_box[i] = i; 
        size[i] = 1;     
    }

    /* ======================= PART 1 UP TO THIS POINT ======================
    *
     * Here is where pt2 differs - i will have to go through them all now
     * or at least prepare to go through them all! I will union them the same, 
     * but keep a count of the number of sets - which starts out being the 
     * same as boxes in the beginning. Every time i create a union, i decrease
     * the num, and as i get down to 1 i am done! The rest of the logic from 
     * pt1 is then removable!
     */
    int num_sets = count;

    for (int i = 0; i < num_d; ++i) {
        int a = pairs[i].ia;
        int b = pairs[i].ib;

        int ra = find(first_box, a);
        int rb = find(first_box, b);

        if (ra != rb) {
            union_sets(first_box, size, a, b);
            num_sets--;

            if (num_sets == 1) {
                // THIS IS THE ANSWER FOR PART TWO
                int x1 = boxes[a].x;
                int x2 = boxes[b].x;

                uint64_t answer = (uint64_t)x1 * (uint64_t)x2;

                INFO("PART 2 answer = %lld", answer);

                break;
            }
        }
    }

    free(pairs);
    free(boxes);
    return 0;
}
