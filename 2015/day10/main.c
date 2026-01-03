#include <blackbox.h>
#include <stdlib.h>
#include <string.h>

void look_see(char *input, char *str){
    int i=0, n_i=0, n=1;
    char c;

    while((c=input[i])!='\0'){
        n=1;
        while(c==input[++i]) n++;
        str[n_i++] = (char)(n+'0');
        str[n_i++] = c;
    }
}

int main(int argc, char** argv) {
    init_log(LOG_DEFAULT);

    char *real = "1113222113";

    char *input = calloc(5000000, sizeof(char));
    char *str = calloc(5000000, sizeof(char));

    memcpy(input, real, strlen(real));
    int count = 50; // 40 for part 1
    int len;
    while(count--){
        look_see(input, str);

        len = strlen(str);
        memcpy(input, str,len);

        DEBUG("new string is %s", input);
        INFO("new string is %d digits",len );
    }

    shutdown_log();
    return 0;
}
