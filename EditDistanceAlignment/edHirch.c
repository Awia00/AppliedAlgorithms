#include "linkedList.c"
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

mychar *strsub(mychar *s, int start, int end) {
    mychar* sub = calloc(end - start + 1, sizeof *sub);
    memcpy(sub, s, end - start);
    return sub;
}

void print_list(node *res)
{
    printf("\n[");
    while(res)
    {
        printf("%d,%d", res->x, res->y);
        if(has_next(res))
        {
            printf("; ");
        }
        res = res->next;
    }
    printf("]\n");
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

node *levenshtein_distance(mychar *a, mychar *b, int startX, int startY)
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

  if (strcmp(a, b) == 0) {
      int i;
      node *res = 0;
      for(i = alen-1; i>=0; i--)
      {
          res = add_node_front(res, startX + i, startY + i);
      }
       return res;
  }

  int *m = (int*) calloc (N * M, sizeof *m);

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
  node *out = 0;
  out = add_node_front(out, alen+startX, blen+startY);
  int i = alen, j = blen;
  while (i > 0 && j > 0) {
    int current = m[M(i, j, N, M)],
        up      = m[M(i-1, j, N, M)],
        diag    = m[M(i-1, j-1, N, M)],
        left    = m[M(i, j-1, N, M)];
    if (up + 1 == current) {
      out = add_node_front(out, startX + i-1, startY + j);
      i--;
    } else if (diag + 1 == current || (diag == current && a[i-1] == b[j-1])) {
      out = add_node_front(out, startX + i-1, startY + j-1);
      i--;
      j--;
    } else if (left + 1 == current) {
      out = add_node_front(out, startX + i, startY + j-1);
      j--;
    } else {
      printf("You done goofed!\n");
      exit(45);
    }
  }

  if (j!=0) {
      // a is empty, so fill rest of length with 'b'.
      for(i = j; i>0; i--)
      {
          out = add_node_front(out, startX, startY + i);
      }
  } else if (i!=0) {
      // see above.
      for(j = i; j > 0; j--)
      {
          out = add_node_front(out, startX + j, startY);
      }
  }
  return out;
}

int *nw_score(mychar *x, mychar *y){
    int xlen = strlen(x), ylen = strlen(y), i, j, N = xlen+1, M = ylen+1;
    int *v0 = calloc(ylen + 1, sizeof *v0);
    int *v1 = calloc(ylen + 1, sizeof *v1);

#ifndef CODEJUDGE
    printf("x: %s\n", x);
    printf("y: %s\n", y);
#endif

    for(j=1;j<M;j++) v0[j] = v0[j-1] + 1;

#ifndef CODEJUDGE
        for(j=0; j<M; j++) {
            printf("%3d", v0[j]);
        }
        printf("\n");
#endif
    for(i=1;i<N;i++) {
        v1[0] = v0[0] + 1;
        for(j=1;j<M;j++) {
            int sub = v0[j-1] + (x[i-1] == y[j-1] ? 0 : 1),
                del = v0[j] + 1,
                ins = v1[j-1] + 1;
            v1[j] = MIN(sub, MIN(del, ins));
        }
        int *swap = v1;
        v1 = v0;
        v0 = swap;
#ifndef CODEJUDGE
        for(j=0; j<M; j++) {
            printf("%3d", v0[j]);
        }
        printf("\n");
#endif
    } 

    free(v1);

#ifndef CODEJUDGE
    printf("\n\n");
#endif

    return v0;
}


node *hirchbergs_align_rec(mychar *x, mychar *y, int startX, int startY)
{
    int xlen = strlen(x);
    int ylen = strlen(y);
    // const int N = xlen + 1;
    // const int M = ylen + 1;
    
    if (xlen == 0 || ylen == 0 ||strcmp(x, y) == 0) {
       return levenshtein_distance(x,y, startX, startY);
    }
    else if (xlen <= 2 || ylen <= 2) {
        return levenshtein_distance(x, y, startX, startY);
    }

    int xmid = xlen/2;
    mychar *left = strsub(x, 0, xmid + 1); // TODO: Check if +1
    mychar *rev_right = strrev(x+xmid, xlen-xmid);
    mychar *rev_y = strrev(y, ylen);

#ifndef CODEJUDGE
    printf("left: %s\n", left);
    printf("rev_right: %s\n", rev_right);
    printf("rev_y: %s\n", rev_y);
#endif

    int *scoreL = nw_score(left, y);
    int *scoreR = nw_score(rev_right, rev_y);

    int i,
        current_min=scoreL[0] + scoreR[ylen],
        min_index = 0;

#ifndef CODEJUDGE
        printf("%3d", current_min);
#endif

    for(i = 1; i <= ylen; i++)
    {
        int score = scoreL[i] + scoreR[ylen-i];

        // Hard > is used to get the topmost 'best' value.
        if(current_min > score)
        {
            current_min = score;
            min_index = i;
        } 
#ifndef CODEJUDGE
        printf("%3d", score);
#endif
    }
    int ymid = min_index;
#ifndef CODEJUDGE
    printf("\n(xmid,ymid): (%d,%d)\n", xmid, ymid);
#endif
    mychar *ytop = strsub(y, 0, ymid + 1); // TODO: Check if +1
    node *left_result = hirchbergs_align_rec(left, ytop, startX, startY); // left result
    node *right_result = hirchbergs_align_rec(x + xmid, y + ymid, startX+xmid, startY+ymid);

#ifndef ONLINE_JUDGE
    print_list(left_result);
    print_list(right_result);
#endif

    get_last(left_result)->next = right_result->next;

    free(ytop);
    return left_result;
}

char *hirchbergs_align(mychar *x, mychar *y)
{
    node *prev = hirchbergs_align_rec(x, y, 0, 0);
    
#ifndef ONLINE_JUDGE
    print_list(prev);
#endif

    char *result = calloc(node_len(prev)+1, sizeof *result);
    int i =0;
    while(prev)
    {
        node *next = prev->next;
        if(next)
        {
            if(prev->x == next->x)
            {
                if(prev->y == next->y)
                {
                    // duplicate value go to next.
                } else
                {
                    result[i++] = 'b';
                    // y movement == a
                }
            } else
            {
                if(prev->y == next->y)
                {
                    result[i++] = 'a';
                    // x movement == b
                } else
                {
                    result[i++] = '|';
                    //diag
                }
            }
        }
        prev = next;
    }
    return result;
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
