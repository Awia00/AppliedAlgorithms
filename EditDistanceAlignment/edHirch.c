#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define MIN(a,b) ((a) < (b) ? a : b)
#define M(i, j, N, M) ((i) * (M) + (j))

typedef int myindex;
typedef char mychar;

mychar *strrev(mychar *s, int slen) {
   mychar *res = calloc((slen + 1), sizeof *res);
   int i;
   for (i=0; i<slen; i++) {
     res[i] = s[slen - i - 1];
   }
   return res;
}

void print_arr(int *m, int N, int M, mychar *a, mychar *b)
{
  int i, j;
  for(i = -1; i<N; i++)
  {
    for(j = -1; j<M; j++)
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
        printf("%3d", m[M(i, j, N, M)]);
      }
    }
    printf("\n");
  }
}

void fill_cost_table(int *m, int N, int M, mychar *a, mychar *b)
{
  int i,j;
  for (i = 0; i < N; i++)
      m[M(i, 0, N, M)] = i;

  for (j = 0; j < M; j++)
      m[M(0, j, N, M)] = j;

#ifndef CODEJUDGE
  printf("\nFirst row and column\n");
  print_arr(m, N, M, a, b);
#endif

  for (i = 1; i < N; i++)
  {
      for (j = 1; j < M; j++)
      {
          m[M(i, j, N, M)] = MIN(
            m[M(i-1, j, N, M)] + 1, MIN(
            m[M(i, j-1, N, M)] + 1,
            m[M(i-1, j-1, N, M)] + ((a[i-1] == b[j-1]) ? 0 : 1))
          );
      }
  }
}

char *levenshtein_distance(mychar *a, mychar *b)
{
  //swap a and b : vi kan også bare bytte om på parameter rækkefølgen - but for now this is it
  mychar *swap = b;
  b = a;
  a = swap;

  int alen = strlen(a);
  int blen = strlen(b);
  const int N = alen + 1;
  const int M = blen + 1;
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

  int *m = (int*) calloc (N * M, sizeof *m);
  //int *m = malloc((alen + 1) * (blen + 1) * sizeof *m);

#ifndef CODEJUDGE
  printf("\nEmpty matrix\n");
  print_arr(m, N, M, a, b);
#endif

  fill_cost_table(m, N, M, a, b);

#ifndef CODEJUDGE
  printf("\nMatrix filled out\n");
  print_arr(m, N, M, a, b);
#endif

  // backtrace
  int i = alen, j = blen;
  myindex resultindex = 0;
  while (i > 0 && j > 0) {
    int current = m[M(i, j, N, M)],
        up      = m[M(i-1, j, N, M)],
        diag    = m[M(i-1, j-1, N, M)],
        left    = m[M(i, j-1, N, M)];
    if (up + 1 == current) {
      out[resultindex++] = 'b';
      i--;
    } else if (diag + 1 == current || (diag == current && a[i-1] == b[j-1])) {
      out[resultindex++] = '|';
      i--;
      j--;
    } else if (left + 1 == current) {
      out[resultindex++] = 'a';
      j--;
    } else {
      printf("You done goofed!\n");
      exit(45);
    }
  }

  if (j!=0) {
      // a is empty, so fill rest of length with 'b'.
      memset(out + resultindex, 'a', j);
      resultindex += j;
      j = 0;
  } else if (i!=0) {
      // see above.
      memset(out + resultindex, 'b', i);
      resultindex += i;
      i = 0;
  }

  return strrev(out, resultindex);
}

int *nw_score(mychar *x, mychar *y){
    int *test;
    return test;
}


char *hirchbergs_align(mychar *x, mychar *y)
{
    int xlen = strlen(x);
    int xmid = xlen/2;
    int ylen = strlen(y);
    // const int N = xlen + 1;
    // const int M = ylen + 1;

    char *out = calloc(xlen + ylen + 1, sizeof *out);
    
    if (strcmp(x, y) == 0) {
       memset(out, '|', xlen);
       return out;
    }

    if (xlen == 0) {
        memset(out, 'b', ylen);
        return out;
    }
    if (ylen == 0) {
        memset(out, 'a', xlen);
        return out;
    }

    if (xlen == 1 || ylen == 1) return levenshtein_distance(x, y);

    mychar* left = calloc(xmid+1, sizeof *left);
    mychar* right = calloc(xmid+1, sizeof *right);
    memcpy(left, x, xmid);
    int *scoreL = nw_score(left, y);
    int *scoreR = nw_score(strrev(x+xmid, xlen-xmid), strrev(y, ylen));

    int i,
        current_min=scoreL[0] + scoreR[ylen-1],
        min_index = 0;
    for(i = 1; i < ylen; i++)
    {
        int score = scoreL[i] + scoreR[ylen-i-1];
        if(current_min > score)
        {
            current_min = score;
            min_index = i;
        } 
    }
    int ymid = min_index;
    
    mychar* ytop = calloc(ymid+1, sizeof *ytop);
    memcpy(ytop, y, ymid);
    out = hirchbergs_align(left, ytop); // right result
    mychar *right_result = hirchbergs_align(x + xmid + 1, y + ymid + 1);
    strcat(out, right_result);
    return out;
}


// Needed for seed method
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

  char *result = hirchbergs_align(a, b);
  printf("%s\n", result);
  return 0;
}
