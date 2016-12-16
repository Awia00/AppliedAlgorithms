#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <math.h>


int question1()
{
    int result;
    int n, m;
    result = scanf("%d", &n); 
    result = scanf("%d", &m);
    
    int u, v;
    int* verticies = (int*) calloc(sizeof verticies, n);
    for(int i = 0; i<m; i++)
    {
        result = scanf("%d", &u);
        result = scanf("%d", &v);
        verticies[u]++;
        verticies[v]++;
    }
    
    int sum = 0;
    for(int i = 0; i<n; i++)
    {
        sum += verticies[i]*verticies[i];
    }
    printf("%d", sum);
    return result;
}

int question1_opt()
{
    int result;
    int n, m;
    result = scanf("%d", &n); 
    result = scanf("%d", &m);
    
    int u, v;
    int* verticies = (int*) calloc(sizeof verticies, n);
    int sum = 0;
    for(int i = 0; i<m; i++)
    {
        result = scanf("%d", &u);
        result = scanf("%d", &v);
        verticies[u]++;
        verticies[v]++;
        sum += verticies[u]*2-1;
        sum += verticies[v]*2-1;
    }
    printf("%d", sum);
    return result;
}


int main(int argc, char **argv) {
    //question1();
    question1_opt();
}