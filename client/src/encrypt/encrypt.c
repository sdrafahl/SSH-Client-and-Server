#include <string.h>
#include "encrypt.h"

int shift(char* decrypt, int n, int size);

int encryptSlide(char* decrypt) {
    return shift(decrypt, encryptShift, strlen(decrypt));
}

int decryptSlide(char* decrypt, int size) {
    return shift(decrypt, -1 * encryptShift,size);
}

int shift(char* decrypt, int n, int size) {
    int x;
    for(x=0;x<size;x++) {
        decrypt[x] = decrypt[x] + n;
    }
    return 0;
}
