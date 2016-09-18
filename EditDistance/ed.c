#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef int myindex;
typedef char mychar;

mychar *a, *b;

void edFullArray(myindex lena, myindex lenb, long seed) {
  srand48(seed);
  a = malloc((lena + 1) * sizeof a);
  a[lena] = 0;
  b = malloc((lenb + 1) * sizeof b);
  b[lenb] = 0;
  int i,j;
  for(i=0;i<lena;i++) a[i] = (lrand48() & 15) + 'a';
  for(j=0;j<lenb;j++) b[j] = (lrand48() & 15) + 'a';
}
#define MIN(a,b) ((a) < (b) ? a : b)

int LevenshteinDistance(mychar *s, mychar *t)
{
  if (strcmp(s,t) == 0) return 0;

  int tlen = strlen(t);
  int slen = strlen(s);

  if (slen == 0) return tlen;
  if (tlen == 0) return slen;

  int v0[tlen + 1];
  int v1[tlen + 1];

  int i,j;
  for (i = 0; i < tlen + 1; i++)
      v0[i] = i;

  for (i = 0; i < slen; i++)
  {
    v1[0] = i + 1;
    for (j = 0; j < tlen; j++)
    {
      v1[j + 1] = MIN(v1[j] + 1, MIN(v0[j + 1] + 1, v0[j] + ((s[i] == t[j]) ? 0 : 1)));
    }
    memcpy(v0, v1, (tlen + 1) * sizeof *v0);
  }

  return v1[tlen];
}

int main(int argc, char **argv) {
  if (argc == 3) { // Two strings
    a = argv[1];
    b = argv[2];
  } else if (argc == 4) {
    myindex lena = atoi(argv[1]);
    myindex lenb = atoi(argv[2]);
    long seed = atol(argv[3]);

    edFullArray(lena, lenb, seed);
  } else {
    printf("Wrong amount of arguments!\n");
    exit(2);
  }

  int result = LevenshteinDistance(a, b);
  printf("%d\n", result);
  return 0;
}
