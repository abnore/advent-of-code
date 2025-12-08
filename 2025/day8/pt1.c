#include "main.h"
#include <blackbox.h>
#include <stdlib.h>
#include <stdint.h>

/* returns squared distance between j1 and j2 (uint64_t) */
static inline uint64_t check_distance(Jbox j1, Jbox j2){
    int64_t a, b, c;
    a = (int64_t)j1.x-(int64_t)j2.x;
    b = (int64_t)j1.y-(int64_t)j2.y;
    c = (int64_t)j1.z-(int64_t)j2.z;
    return (uint64_t)((a*a) + (b*b) + (c*c));
}
/* The compare function used by qsort for pairs */
static int cmp_pair(const void *a,const void *b) {
    const Pair *p1 = a, *p2 = b;
    if(p1->dist < p2->dist) return -1;
    if(p1->dist > p2->dist) return  1;
    return 0;
}
/* The compare function used by qsort for sizes */
static int cmp_size(const void *a,const void *b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    return y-x;
}

/* I chose a simple recursive find function to return the root of the set this
 * index belongs to, from the DSU article */
static int find(int *parent, int index){
    TRACE("parent[%i]=%i", index, parent[index]);
    if(parent[index] == index) 
        return index;
    return find(parent, parent[index]);
}

/* From the DSU article. I check sizes first, so the smaller tree gets attached
 * under the larger one. This keeps things from becoming lopsided. */
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
/* I originally tried to solve this by making each box point to its closest box
 * and detecting loops that way. I tried linked lists, and even wondered about
 * AVL trees etc. It got messy fast, and I dropped it. The code now uses DSU
 * instead, which is much cleaner for tracking connected sets.
 * 
 * For the test its only 190 possible distances In the real input (of
 * 1000 lines) there are 499,5k distances. No problem to loop over. N(N-1)/2
 * unique distances. 
 *
 * Found an article of Disjoint Set Union or DSU:
 * https://cp-algorithms.com/data_structures/disjoint_set_union.html
 *
 * I couldn't think of another way to do this without increasing the complexity 
 * to unnecessary levels. I will need a find and union functions - written above
 */

int pt1(FILE* fp){
    Jbox* boxes = NULL;
    size_t count = 0;
    size_t capacity = 0;

    char line[256];

    while (fgets(line, sizeof(line), fp)) {

        if (count == capacity) {
            capacity = capacity == 0 ? 16 : capacity << 1; // I will increase by a factor of 2
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
    INFO("Read in a total of %zu boxes", count);
    /* I have now a collection of all the junction boxes. 
     * Looping through every one in relation to every one else
     * will yield a list of all possible distances. This will 
     * stored as Pairs, and after that we will sort the distances */

    int num_d = (count*(count-1))/2, num_pairs=0;
    INFO("I have calculated there to be a total of %d pairs", num_d);

    Pair *pairs = malloc(num_d*sizeof(Pair));
    for (int i=0; i<count; ++i)
    {
        for(int ii=i+1; ii<count; ++ii){
            uint64_t dist = check_distance((boxes[i]), (boxes[ii]));
            Pair *p = &pairs[num_pairs++]; 
            p->dist = dist;
            p->ia = i;
            p->ib = ii;
            TRACE("Distance %3i box[%2i] and box[%2i] is %llu",num_pairs, i, ii, dist);
        }
    }
    DEBUG("This created %i many pairs", num_pairs);
    ASSERT(num_d == num_pairs, "These numbers should be the same %i!=%i", num_d, num_pairs);
    DEBUG("A pair, e.g. p[18]: now has squared dist => p.distance=%llu, and two pointers to their boxes", pairs[18].dist);
  
    /* Now we sort the list, therefore we only need to make one pass to connect them. For that the best way is qsort
     * I write a simple comparison function at the expected signature and call qsort on the pairs
     */
    qsort(pairs, (size_t)num_pairs, sizeof(Pair), cmp_pair);

    for(int i=0; i<num_pairs; ++i){
        TRACE("pair %i: %llu - boxes[%i] -- boxes[%i]", i, pairs[i].dist, pairs[i].ia, pairs[i].ib);
    }

    /* Now we have to create the circuits (unions) 
     * They way i will do that is keep a count of unions in an array that points
     * via indexes and redirects. This way i might have gotten away with no pointers, 
     * although this task needed heavy debugging and logging
     */
    
    int first_box[count]; // Here i will store the index into boxes[i]
    int size[count];      // And i will keep track of the sizes here

    for(int i=0; i<count; ++i) {
        first_box[i] = i;   // every box starts as its own root
        size[i] = 1;     // each box starts in its own group
    }

    /*
     * After making the ten shortest connections, there are 11 circuits: one
     * circuit which contains 5 junction boxes, one circuit which contains 4
     * junction boxes, two circuits which contain 2 junction boxes each, and
     * seven circuits which each contain a single junction box. Multiplying
     * together the sizes of the three largest circuits (5, 4, and one of the
     * circuits of size 2) produces 40.
     */
    // Limit so i know if the test cases is correct (10 for test, 1000 for real)
    int LIMIT;
    if(count==20) LIMIT = 10; // test case
    else LIMIT = 1000; // real input

    for (int i = 0; i < LIMIT; ++i) {
        int a = pairs[i].ia;   // index of Jbox A
        int b = pairs[i].ib;   // index of Jbox B

        union_sets(first_box, size, a, b);
    }

    /* I need to know how many roots i have after using union */ 
    int num_roots = 0;
    for (int i = 0; i < count; ++i) {
        if (first_box[i] == i) {
            num_roots++;
        }
    }

    /* Then i will have to dynamically store the groups (which can be an arbitrary size) 
     * and store the size of these groups 
     */
    int *groups = malloc(num_roots * sizeof(int));
    int idx = 0;

    for (int i = 0; i < count; ++i) {
        if (first_box[i] == i) {        // root
            groups[idx++] = size[i]; // store the size
            TRACE("size[%i] = %i",i, size[i]);
        }
    }
    /* If i am only taking the 3 largest groups for multiplying i will need to sort them!
     * Create a new function for qsort called cmp_size and sort based on numbers alone 
     */
    qsort(groups, num_roots, sizeof(int), cmp_size);

    long long result = 1;
    for (int i = 0; i < 3; ++i) {
        result *= groups[i];
    }

    INFO("PART 1 answer = %llu", result);

    free(groups);
    free(pairs);
    free(boxes);

    return 0;
}
