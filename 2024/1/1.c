#include <stdio.h>
#include <stdlib.h>

// Comparison function
int compare(const void* a, const void* b) {
        return (*(int*)a - *(int*)b);
}

int main()
{
        FILE *fp=fopen("input.in", "r");
        int n = 1000;
        int l_list[n];
        int r_list[n];

        for(int i = 0; i < n; ++i)
        {
                fscanf(fp,"%i\t%i", &l_list[i], &r_list[i]);
        }

        qsort(l_list, n, sizeof(int), compare);
        qsort(r_list, n, sizeof(int), compare);

        int sum = 0;
        for(int i = 0; i < n; ++i)
        {
                sum += abs(l_list[i] - r_list[i]);
        }

        printf("the sum is %i\n", sum);
/********************** part two *************************/

        int sum2 = 0;
        for(int i = 0; i < 1000; ++i)
        {
                int num = l_list[i];
                int count = 0;

                for(int j = 0; j < 1000; ++j)
                {
                        if(num == r_list[j]){
                                count++;
                        }
                }
                sum2 += (count * l_list[i]);
        }

        printf("%i\n", sum2);
        return 0;
}
