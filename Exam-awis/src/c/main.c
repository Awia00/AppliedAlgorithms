#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <math.h>

#define BUFFER_SIZE BUFSIZ
void question1()
{
    int input;
    while(scanf("%d", &input) >= 1)
    {       
        printf("%d\n", input);
    }
}

void question3()
{
    char input[BUFFER_SIZE];
    
    do{
        fgets(input, BUFSIZ, stdin);
        printf("%s", input);
    }
    while ( strcmp(input, "EOF") != 0);
}


int main(int argc, char **argv) {
    //question1();
    question3();
}