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

char *strrev(char *s, int slen) {
   char *res = malloc(slen * sizeof *res);
   int i;
   for (i=0; i<slen; i++) {
     res[i] = s[slen - i - 1];
   }
   return res;
}

#define M(i, j, alen) (i * alen + j)
void print_arr(int *m, int alen, int blen, char *a, char *b)
{
  int i, j;
  for(i = -1; i<=alen; i++)
  {
    for(j = -1; j<=blen; j++)
    {
      if(i==-1) // print the first row of letters
      {
        if(j <= 0)
          printf("%3c",' ');
        else 
          printf("%3c",b[j-1]);
      }
      else if(j==-1) // print the first column of letters
      {
        if(i <= 0)
          printf("%3c",' ');
        else
          printf("%3c", a[i-1]);
      }
      else{ // print the matrix
        printf("%3d", m[i * (blen + 1) + j]);
      } 
    }
    printf("\n");
  }
}

#define M(i, j, alen) (i * alen + j)

char *LevenshteinDistance(mychar *a, mychar *b)
{
  //swap a and b
  mychar *swap = b;
  b = a;
  a = swap;

  int alen = strlen(a); 
  int blen = strlen(b); 
#ifndef CODEJUDGE
  printf("\nalen: %d", alen);
  printf("\nblen: %d", blen);
#endif

  char *out = calloc(alen + blen, sizeof *out);

  if (strcmp(a, b) == 0) {
       memset(out, '|', alen);
       return out;
  }

  if (alen == 0) {
       memset(out, 'b', blen);
       return out;
  }
  if (blen == 0) {
       memset(out, 'a', alen);
       return out;
  }

  int *m = (int*) calloc ((alen + 1) * (blen + 1), sizeof *m);
  //int *m = malloc((alen + 1) * (blen + 1) * sizeof *m);

#ifndef CODEJUDGE
  printf("\nEmpty matrix\n");
  print_arr(m, alen, blen, a, b);
#endif

  // fill matrix
  int i,j;
  for (i = 0; i <= alen; i++)
      m[i*(blen+1)] = i;

  for (j = 0; j <= blen; j++)
      m[j] = j;

#ifndef CODEJUDGE
  printf("\nFirst row and column\n");
  print_arr(m, alen, blen, a, b);
#endif
  for (i = 1; i <= alen; i++)
  {
      for (j = 1; j <= blen; j++)
      {
          m[i * (blen + 1) + j] = MIN(m[(i-1) * (blen + 1) + j] + 1, MIN(m[i * (blen + 1) + j-1] + 1, m[(i-1) * (blen + 1) + j-1] + ((a[i-1] == b[j-1]) ? 0 : 1)));
      }
  }

  // Printing the matrix.

#ifndef CODEJUDGE
  printf("\nMatrix filled out\n");
  print_arr(m, alen, blen, a, b);
#endif

  // backtrace
  i = alen; j = blen;
  myindex resultindex = 0;
  while (i > 0 || j > 0) {
    if (i==0) {
       // a is empty, so fill rest of length with 'b'.
       memset(out + resultindex, 'a', j);
       resultindex += j;
       j = 0;
    } else if (j==0) {
       // see above.
       memset(out + resultindex, 'b', i);
       resultindex += i;
       i = 0;
    } else {
      int current = m[i     * (blen + 1) + j], 
          up      = m[(i-1) * (blen + 1) + j], 
          diag    = m[(i-1) * (blen + 1) + j-1], 
          left    = m[i     * (blen + 1) + j-1];
      if (diag == current && diag <= up && diag <= left)
      {
        out[resultindex++] = '|';
        i--;
        j--;
      }
      else if (up <= diag && up <= left) {
        out[resultindex++] = 'b';
        i--;
      } else if (diag <= up && diag <= left) {
        out[resultindex++] = '|';
        i--;
        j--;
      } else if (left <= up && left <= diag) {
        out[resultindex++] = 'a';
        j--;
      } else {
        printf("You done goofed!\n");
        exit(45);
      }
    }
  }
  return strrev(out, resultindex);
}

int main(int argc, char **argv) {
  if (argc == 3) { // Two strings
    a = argv[1];
    b = argv[2];
  } else if (argc == 4) {
    myindex slen = atoi(argv[1]);
    myindex tlen = atoi(argv[2]);
    long seed = atol(argv[3]);

    edFullArray(slen, tlen, seed);
  } else {
    printf("Wrong amount of arguments!\n");
    exit(2);
  }

  char *result = LevenshteinDistance(a, b);
  printf("%s\n", result);
  return 0;
}
