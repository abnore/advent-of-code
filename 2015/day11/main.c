#include <blackbox.h>
#include <string.h>

#define LEN 8

int pair(const char *s){
    int pair1=0;
    char first_pair;
    char ptr1,ptr2;
    for(int i=0;i<LEN-1; i++){
        ptr1=s[i];
        ptr2=s[i+1];

        if(ptr1==ptr2){
            if(!pair1) {
                first_pair = ptr1;
                pair1=1;
                TRACE("%c%c", ptr1, ptr2);
            } else if(pair1){
                if(ptr1==first_pair) continue;
                TRACE("%c%c", ptr1, ptr2);
                return 1;
            }
        }
    }
    return 0;
}
int no(const char *s){
    for(int i=0; i<LEN; i++)
        if(s[i]=='l' || s[i]=='i' || s[i]=='o')
            return 1;
    return 0;
}
int straight(const char *s){
    for(int i=0; i<LEN-2; i++){
        char first = s[i];
        char sec = s[i+1];
        char tri = s[i+2];

        if(tri <= 0x7a && tri == sec+1 && sec == first+1) {
            TRACE("%c%c%c", first, sec, tri);
            return 1;
        }
    }
    return 0;
}
// a 97/x61 z 122/x7a
void increment(char *s){
    int len = strlen(s);
    ASSERT(len==8, "string to short %i", len);

    int i=LEN-1; 

    while(i>=0){
        s[i]++;
        if(s[i]>0x7a){
            s[i]=0x61; 
        } else {
            break;
        }
        --i;
    }
}
int main(int argc, char** argv) {
    init_log(LOG_DEFAULT);
    char str[] = "hxbxwxba"; // part 1 hxbxxyzz - part 2 hxcaabcc

    while(1){
        increment(str);
        if(no(str)) continue;
        if(straight(str) && pair(str)) break;
    }

    INFO("%s", str);

    shutdown_log();
    return 0;
}
