#include <string.h>
#include "encrypt.h"

int shift(char* decrypt, int n);

int encryptSlide(char* decrypt) {
    return shift(decrypt, encryptShift);
}

int decryptSlide(char* decrypt) {
    return shift(decrypt, -1 * encryptShift);
}

int shift(char* decrypt, int n) {
    int x;
    for(x=0;x<strlen(decrypt);x++) {
        decrypt[x] = decrypt[x] + n;
    }
    return 0;
}
