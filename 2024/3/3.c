#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

mul(xxx,xxx)

int main()
{
        FILE *fp=fopen("input.in", "r");

        char buf[1024];
        char ch;
        int letters, digits = 0;

        while ((ch = fgetc(fp)) != EOF) {

                if (isalpha(ch)) {
                        letters++;
                } else if (isdigit(ch)) {
                        digits++;
                }
        }

        printf("digits %i, letters %i\n", digits, letters);
        return 0;
}
