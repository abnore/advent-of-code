#include "main.h"
#include <blackbox.h>
#include <string.h>
#include <CommonCrypto/CommonCrypto.h>

int pt1(const char *str) {

    char string[33]={0};
    unsigned char digest[CC_MD5_DIGEST_LENGTH];

    for(int i=0; i<INT_MAX; ++i){
        sprintf(string, "%s%d", str,i);
        CC_MD5(string, (CC_LONG)strlen(string), digest);
        if (!digest[0] && !digest[1] &&
            (digest[2] & 0xF0) == 0x00) // mask out the first nibble
            return i;
    }
    return -1;
}
