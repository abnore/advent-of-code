#include "main.h"
#include <blackbox.h>

#define up(a)       do{if(a>3) a-=3;}while(0)
#define down(a)     do{if(a<7) a+=3;}while(0)
#define left(a)     do{if( a!= 1 && a!=4 && a!=7) a-=1;}while(0)
#define right(a)    do{if( a!= 3 && a!=6 && a!=9) a+=1;}while(0)
#define store(a,b)  (a = a*10 + b)

int pt1(FILE* fp)
{
    int start = 5;
    int pin=0;

    char *line = NULL;
    size_t cap = 0;
    ssize_t n;

    while ((n = getline(&line, &cap, fp)) > 0) {
        for(int i=0; i<n-1; i++){
            switch(line[i]){
                case 'U': up(start);    break;
                case 'D': down(start);  break;
                case 'R': right(start); break;
                case 'L': left(start);  break;
                default:
                    printf("Something has gone wrong here\n");
                    break;
            }
        }
        store(pin, start);
    }
    free(line);

    return pin;
}

/* Worth keeping in mind, this style of coding. Conditional subtractions, and
 * having the keypad broken down into a math equation. I thought of this after
 * my implementation, and tested it. leaving it here as an alternative.

         case 'U': y -= (y > 0); break;
         case 'D': y += (y < 2); break;
         case 'L': x -= (x > 0); break;
         case 'R': x += (x < 2); break;
         case '\n':
            pin = pin * 10 + 3*y + x + 1; break;
*/
