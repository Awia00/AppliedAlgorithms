#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <math.h>


int question3()
{
    int result, n;
    result = scanf("%d", &n); 
    
    float u, v;
    float* entitites = (float*) calloc(sizeof entitites, n);
    for(int i = 0; i<n; i++)
    {
        result = scanf("%f", &u);
        result = scanf("%f", &v);
    }
    return result;
}



int main(int argc, char **argv) {
    question3();
}