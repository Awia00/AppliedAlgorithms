#include "hashlib.h"

const int BITS = 32;
const unsigned int a[] = {
            0X21ae4036, 0X32435171, 0Xac3338cf, 0Xea97b40c, 0X0e504b22, 0X9ff9a4ef, 0X111d014d, 0X934f3787, 0X6cd079bf,
            0X69db5c31, 0Xdf3c28ed, 0X40daf2ad, 0X82a5891c, 0X4659c7b0, 0X73dc0ca8, 0Xdad3aca2, 0X00c74c7e, 0X9a2521e2,
            0Xf38eb6aa, 0X64711ab6, 0X5823150a, 0Xd13a3a9a, 0X30a5aa04, 0X0fb9a1da, 0Xef785119, 0Xc9f0b067, 0X1e7dde42,
            0Xdda4a7b2, 0X1a1c2640, 0X297c0633, 0X744edb48, 0X19adce93 };

int h(int x) {
    int res = 0;
    for (int i = 0; i < BITS; i++) {
        if (__builtin_popcount(a[i] & x) & 1) {
            res |= (1 << (BITS - i - 1));
        }
    }
    return res;
}

int h_first(int x) {
    int res = 1;
    for (int i = 0; i < BITS; i++) {
        if (!(__builtin_popcount(a[i] & x) & 1)) {
            break;
        }
        res++;
    }
    return res;
}

int f(int x) {
    return ((x*0Xbc164501) & 0X7fe00000) >> 21;
}