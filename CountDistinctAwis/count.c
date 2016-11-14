#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <math.h>

const int BIT_AMT = 32;
const unsigned int A[] = {
    0x21ae4036,    0x32435171,    0xac3338cf,    0xea97b40c,
    0x0e504b22,    0x9ff9a4ef,    0x111d014d,    0x934f3787,
    0x6cd079bf,    0x69db5c31,    0xdf3c28ed,    0x40daf2ad,
    0x82a5891c,    0x4659c7b0,    0x73dc0ca8,    0xdad3aca2,
    0x00c74c7e,    0x9a2521e2,    0xf38eb6aa,    0x64711ab6,
    0x5823150a,    0xd13a3a9a,    0x30a5aa04,    0x0fb9a1da,
    0xef785119,    0xc9f0b067,    0x1e7dde42,    0xdda4a7b2,
    0x1a1c2640,    0x297c0633,    0x744edb48,    0x19adce93}; 

//
int gf2_hash_count(int x)
{
    int r = 0;
    for(int i = 0; i < BIT_AMT; i++)
    {
        r++;
        if(__builtin_popcount(A[i] & x) & 1) // the first 1 has been found no reason to calculate the rest
            return r;
    }
    return r;
}

// h
int gf2_hash_opt(int x)
{
    int r = 0;
    for(int i = 0; i < BIT_AMT; i++)
    {
        int hash = (__builtin_popcount(A[i] & x) & 1);
        r += hash << (BIT_AMT-i-1);
        if(hash) // the first 1 has been found no reason to calculate the rest
            return r;
    }
    return r;
}

// h
int gf2_hash(int x)
{
    int r = 0;
    for(int i = 0; i < BIT_AMT; i++)
    {
        r += (__builtin_popcount(A[i] & x) & 1) << (BIT_AMT-i-1);
    }
    return r;
}

// f
int heuristic_hash(int x) { 
    return ((x*0Xbc164501) & 0X7fe00000) >> 21;
}

double hyperloglog(int m)
{
    int* M = (int*)calloc(m, sizeof(int));
    int input;
    while(scanf("%d", &input) == 1)
    {
        int j = heuristic_hash(input);
        // int new = __builtin_clz(gf2_hash_opt(input)) + 1;
        int new = gf2_hash_count(input);
        if(M[j] < new)
        {
            M[j] = new;
        }
    }

    double Z = 0;
    double V = 0;
    for (int i = 0; i < m; i++) {
        
        if (M[i] == 0) 
        {
            V++;
            Z++;
        } else {
            Z += pow(2.0, -M[i]);
        }
    }
    Z = 1/Z;

    double E = m*m*Z*0.7213 / (1.0 + 1.079/m);

    if (E < 2.5*m && V > 0.0) {
        E = m * log(m/V);
    }

    return E;
}

int question1()
{
    int input;
    while(scanf("%d", &input) == 1)
    {
        int j = gf2_hash(input);
        printf("%d\n", j);
    }
    return 0;
}

int question3()
{
    int threshold;
    int exit = scanf("%d", &threshold);
    double result = hyperloglog(1024);
    if (result < threshold) 
        printf("%s\n","below");
    else
        printf("%s\n","above");
    return !exit;
}

int main(int argc, char **argv) {
    return question3();
}