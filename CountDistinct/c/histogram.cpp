#include "hashlib.h"
#include <stdio.h>

int main() {
    int *buckets = new int[32];

    for(int i = 0; i < 100000; i++) {
        buckets[h_first(i + 1) - 1]++;
    }

    for(int i = 0; i < 32; i++) {
        if (buckets[i])
            printf("%d\t%d\n", i+1, buckets[i]);
    }

    return 0;
}