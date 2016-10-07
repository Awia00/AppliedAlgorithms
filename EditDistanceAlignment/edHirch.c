
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

void print_single_arr(int *a, int N)
{
    printf("\n");
    int i;
    for(i = 0; i<N; i++)
    {
        printf("%d, ", a[i]);
    }
}

void print_single_arr_rev(int *a, int N)
{
    printf("\n");
    int i;
    for(i = N-1; i>=0; i--)
    {
        printf("%d, ", a[i]);
    }
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

void levenshtein_distance(mychar *a, mychar *b, int xlen, int ylen, int startX, int startY, int *path_points)
{
    //swap a and b : vi kan også bare bytte om på parameter rækkefølgen - but for now this is it
    mychar *swap = b;
    b = a;
    a = swap;

    int alen = ylen;
    int blen = xlen;
    const int N = alen + 1;
    const int M = blen + 1;

    if (strcmp(a, b) == 0) {
        int i;
        for(i = alen; i>=0; i--)
        {
            path_points[startX + i + startY + i] = startX + i;
        }
        return;
    }

    int *m = (int*) calloc (N * M, sizeof *m);

    fill_cost_table(m, N, M, a, b);

    // backtrace
    int i = alen, j = blen;
    while (i > 0 && j > 0) {
        path_points[startX + j + startY + i] = startX + j;
        int current = m[M(i, j, N, M)],
            up      = m[M(i-1, j, N, M)],
            diag    = m[M(i-1, j-1, N, M)],
            left    = m[M(i, j-1, N, M)];
        if (up + 1 == current) {
            i--;
        } else if (diag + 1 == current || (diag == current && a[i-1] == b[j-1])) {
            i--;
            j--;
        } else if (left + 1 == current) {
            j--;
        } else {
            printf("You done goofed!\n");
            exit(45);
        }
    }
    path_points[startX + j + startY + i] = startX + j;

    if (j!=0) {
        // a is empty, so fill rest of length with 'b'.
        for(i = j-1; i>=0; i--)
        {
            path_points[startX + i + startY] = startX + i;
        }
    } else if (i!=0) {
        // see above.
        for(j = i-1; j >= 0; j--)
        {
            path_points[startX + startY + j] = startX;
        }
    }
}

int *nw_score(mychar *x, mychar *y, int xlen, int ylen){
    int i, j;
    int *v0 = calloc(ylen + 1, sizeof *v0);
    int *v1 = calloc(ylen + 1, sizeof *v1);

    for(j=1;j<=ylen;j++) v0[j] = v0[j-1] + 1;

    for(i=0;i<xlen;i++) {
        v1[0] = v0[0] + 1;
        for(j=0;j<ylen;j++) {
            int sub = v0[j] + (x[i] == y[j] ? 0 : 1),
                del = v0[j+1] + 1,
                ins = v1[j] + 1;
            v1[j+1] = MIN(sub, MIN(del, ins));
        }
        int *swap = v1;
        v1 = v0;
        v0 = swap;
    } 

    free(v1);

    return v0;
}


void hirchbergs_align_rec(mychar *x, mychar *y, int xlen, int ylen, int startX, int startY, int *path_points)
{
    if (xlen <= 100 || ylen <= 100 ||strcmp(x, y) == 0) {
        levenshtein_distance(x, y, xlen, ylen, startX, startY, path_points);
        return;
    }

    int xmid = xlen/2;
    mychar *left = strsub(x, 0, xmid);
    mychar *rev_right = strrev(x+xmid, xlen-xmid);
    mychar *rev_y = strrev(y, ylen);

    int *scoreL; 
    int *scoreR; 
    #pragma omp task shared(scoreL)
    scoreL = nw_score(left, y, xmid, ylen);
    #pragma omp task shared(scoreR)
    scoreR = nw_score(rev_right, rev_y, xlen-xmid, ylen);
    #pragma omp taskwait
    free(rev_right);
    free(rev_y);

    int i,
        current_min=scoreL[0] + scoreR[ylen],
        min_index = 0;

    for(i = 1; i <= ylen; i++)
    {
        int score = scoreL[i] + scoreR[ylen-i];

        // Hard > is used to get the topmost 'best' value.
        if(current_min > score)
        {
            current_min = score;
            min_index = i;
        } 
    }
    free(scoreL);
    free(scoreR);
    int ymid = min_index;

    mychar *ytop = strsub(y, 0, ymid);

    #pragma omp task
    { hirchbergs_align_rec(left, ytop, xmid, ymid, startX, startY, path_points);} // left result
    #pragma omp task
    { hirchbergs_align_rec(x + xmid, y + ymid, xlen - xmid, ylen - ymid, startX+xmid, startY+ymid, path_points);} 
    #pragma omp taskwait

    free(ytop);
    free(left);
}

// Start of recoursive calls
char *hirchbergs_align(mychar *x, mychar *y)
{
    int xlen = strlen(x), ylen = strlen(y);
    int path_points[xlen + ylen + 1];
    int index, k;
    for (k = 0; k < xlen + ylen; k++) {
        path_points[k] = -1;
    }
    path_points[xlen + ylen] = xlen;
    #pragma omp parallel
    #pragma omp single nowait
    hirchbergs_align_rec(x, y, xlen, ylen, 0, 0, path_points);

#ifndef CODEJUDGE
    for(k=0; k <= xlen + ylen; k++) {
       printf("%3d", path_points[k]);
    }
    printf("\n");
#endif

    char *result = calloc(xlen + ylen +1, sizeof *result);
    index = 0;
    int last_i = 0, last_j = 0, i, j;
    for (k = 1; k <= xlen + ylen; k++) {
        i = path_points[k];
        if (path_points[k] != -1) {
            j = k - i;
            if (i == last_i) {
                if (j == last_j) {
                    printf("Goofed 46"); exit(46);
                } else {
                    // j changed.
                    result[index++] = 'b';
                }
            } else {
                if (j == last_j) {
                    // i changed.
                    result[index++] = 'a';
                } else {
                    // both changed
                    result[index++] = '|';
                }
            }
            last_i = i; last_j = j;
        }
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
    free(result);
    return 0;
}
