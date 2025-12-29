#include "main.h"
#include <blackbox.h>
#include <string.h>
#include <CommonCrypto/CommonCrypto.h>

static const char *calc_md5(unsigned char *digest, const char *str) {
    CC_MD5(str, (CC_LONG)strlen(str), digest);
    return str;
}

int pt2(const char *str)
{
    char string[33]={0};
    unsigned char digest[CC_MD5_DIGEST_LENGTH];

    for(int i=0; i<INT_MAX; ++i){
        sprintf(string, "%s%d", str,i);
        calc_md5(digest, string);
        if (!digest[0] && !digest[1] && !digest[2])
            return i;
    }
    return -1;
}
