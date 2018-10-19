#include "encrypt.h"

int shift(char* decrypt, int n);

int encrypt(char* decrypt) {
    return shift(decrypt, encryptShift);
}

int decrypt(char* decrypt) {
    return shift(decrypt, -1 * encryptShift);
}

int shift(char* decrypt, int n) {
    int x;
    for(x=0;x<strlen(decrypt);x++) {
        decrypt[x] + n;
    }
    return 0;
}
