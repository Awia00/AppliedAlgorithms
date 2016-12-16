#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <math.h>

const int n = 1024;
const int bucket_size = 8;
int h(int x){
    return 0x7fffffff &( ((x*32)^(x/2)) + 1 );
}
int h_i(int i, int x)
{
    int r = x;
    for(int j = 1; j <= i; j++)
        r = h(r);
    return r;
}

int x(int j)
{
    return ((1<<(j % 5)) + j*32)*16;
}

int* m(int i)
{
    int* a = calloc(sizeof a, n);
    for(int j = 0; j<n; j++)
    {
        a[j] = h_i(i, x(j));
    }
    return a;
}

void question2_a(int i)
{
    int* a = m(i);
    for(int j = 0; j<n; j++)
    {
        printf("%d\n", a[j]);
    }
}

void question2_b(int i)
{
    int* buckets = calloc(sizeof buckets, 8);
    
    int* a = m(0);
    for(int v = 0; v<i; v++)
    {
        #pragma omp parallel for shared(a, buckets) default(none)
        for(int j = 0; j<n; j++)
        {
            int bucket = a[j]%bucket_size;
            buckets[bucket]++;
            a[j] = h(a[j]);
        }

        printf("%d:\t", v);
        for(int j = 0; j<bucket_size; j++)
        {
            printf("%d(%d)\t", buckets[j], j);
            buckets[j] = 0;
        }
        printf("\n");
    }
}

void question2_c(int i)
{
    int* buckets = calloc(sizeof buckets, 8);
    
    int* a = m(0);
    for(int v = 0; v<i; v++)
    {
        
        int max = 0;
        #pragma omp parallel for shared(a, buckets) default(none)
        for(int j = 0; j<n; j++)
        {
            int bucket = a[j]%bucket_size;
            buckets[bucket]++;
            a[j] = h(a[j]);
        }
        
        for(int j = 0; j<bucket_size; j++)
        {
            if(buckets[j] > max) 
                max = buckets[j];
            buckets[j] = 0;
        }

        printf("%d %d\n", v, max);
        
    }
}






int main(int argc, char **argv) {
    int i = atoi(argv[1]);
    //question2_a(i);
    //question2_b(i);
    question2_c(i);
}