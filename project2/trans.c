/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void trans(int M, int N, int A[N][M], int B[M][N]);
void transpose1(int M, int N, int A[N][M], int B[M][N]);
void transpose2(int M, int N, int A[N][M], int B[M][N]);
void transpose3(int M, int N, int A[N][M], int B[M][N]);
void transpose4(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32) transpose1(M, N, A, B);
    else if ( M == 64) transpose2(M, N, A, B);
    else transpose3(M, N, A, B);
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 
char transpose1_desc[] = "Transpose1";
void transpose1(int M, int N, int A[N][M], int B[M][N])
{
    // the number of variables in this function is 11
    // including loop variables i, j, k
    // and storing variables val0 to val7
    int i, j;
    int k;
    int val0, val1, val2, val3, val4, val5, val6, val7;
    for (i = 0; i < M; i += 8)
	for (j = 0; j < N; j += 8)
	    for (k = i; k < i + 8; k++)
	    {
		val0 = A[k][j + 0];
		val1 = A[k][j + 1];
		val2 = A[k][j + 2];
		val3 = A[k][j + 3];
                val4 = A[k][j + 4];
                val5 = A[k][j + 5];
		val6 = A[k][j + 6];
                val7 = A[k][j + 7];

		B[j + 0][k] = val0;
		B[j + 1][k] = val1;
		B[j + 2][k] = val2;
                B[j + 3][k] = val3;
		B[j + 4][k] = val4;
                B[j + 5][k] = val5;
                B[j + 6][k] = val6;
                B[j + 7][k] = val7;
	    }
}

char transpose2_desc[] = "Transpose2";
void transpose2(int M, int N, int A[N][M], int B[M][N])
{
    // the number of variables in this function is 11
    // including loop variables i, j, k
    // and storing variables val0 to val7
    int i, k, j;
    int val0, val1, val2, val3, val4, val5, val6, val7;
    for (i = 0; i < N; i += 8)
	for (j = 0; j < M; j += 8)
	{
            for (k = i; k < i + 4; k++)
            {
                val0 = A[k][j];
		val1 = A[k][j + 1];
		val2 = A[k][j + 2];
		val3 = A[k][j + 3];
		val4 = A[k][j + 4];
                val5 = A[k][j + 5];
                val6 = A[k][j + 6];
                val7 = A[k][j + 7];

		B[j][k] = val0;
		B[j + 1][k] = val1;
		B[j + 2][k] = val2;
		B[j + 3][k] = val3;
		B[j][k + 4] = val4;
		B[j + 1][k + 4] = val5;
		B[j + 2][k + 4] = val6;
		B[j + 3][k + 4] = val7;
            }
	    for (k = j; k < j + 4; k++)
	    {
		val0 = A[i + 4][k];
		val1 = A[i + 5][k];
		val2 = A[i + 6][k];
		val3 = A[i + 7][k];
		val4 = B[k][i + 4];
		val5 = B[k][i + 5];
		val6 = B[k][i + 6];
		val7 = B[k][i + 7];

		B[k][i + 4] = val0;
		B[k][i + 5] = val1;
		B[k][i + 6] = val2;
		B[k][i + 7] = val3;
		B[k + 4][i] = val4;
		B[k + 4][i + 1] = val5;
		B[k + 4][i + 2] = val6;
		B[k + 4][i + 3] = val7;
	    }
	    for (k = i + 4; k < i + 8; ++k)
	    {
		val0 = A[k][j + 4];
		val1 = A[k][j + 5];
		val2 = A[k][j + 6];
		val3 = A[k][j + 7];

		B[j + 4][k] = val0;
		B[j + 5][k] = val1;
		B[j + 6][k] = val2;
		B[j + 7][k] = val3;
	    }
	}
}

char transpose3_desc[] = "transpose3";
void transpose3(int M, int N, int A[N][M], int B[M][N])
{
    // the number of variables in this function is exactly 12
    // including loop variables i, j, k, l
    // and storing variables val0 to val7
    int i, j, k, l;
    int val0, val1, val2, val3, val4, val5, val6, val7;
    for (i = 0; i + 16 < N; i += 16)
	for (j = 0; j + 16 < M; j += 16)
	    for (k = i; k < i + 16; k++)
	    {
		val0 = A[k][j];
		val1 = A[k][j + 1];
		val2 = A[k][j + 2];
		val3 = A[k][j + 3];
		val4 = A[k][j + 4];
		val5 = A[k][j + 5];
		val6 = A[k][j + 6];
		val7 = A[k][j + 7];

		B[j][k] = val0;
		B[j + 1][k] = val1;
		B[j + 2][k] = val2;
		B[j + 3][k] = val3;
		B[j + 4][k] = val4;
		B[j + 5][k] = val5;
		B[j + 6][k] = val6;
		B[j + 7][k] = val7;

		j += 8;
		val0 = A[k][j];
                val1 = A[k][j + 1];
                val2 = A[k][j + 2];
                val3 = A[k][j + 3];
                val4 = A[k][j + 4];
                val5 = A[k][j + 5];
                val6 = A[k][j + 6];
                val7 = A[k][j + 7];

                B[j][k] = val0;
                B[j + 1][k] = val1;
                B[j + 2][k] = val2;
                B[j + 3][k] = val3;
                B[j + 4][k] = val4;
                B[j + 5][k] = val5;
                B[j + 6][k] = val6;
                B[j + 7][k] = val7;
		j -= 8;
	    }
    for (k = i; k < N; k++)
	for (l = 0; l < M; l++)
	    B[l][k] = A[k][l];

    for (k = 0; k < i; k++)
	for (l = j; l < M; l++)
	    B[l][k] = A[k][l];
}

char transpose4_desc[]="Transpose 4";
void transpose4(int M,int N,int A[N][M],int B[M][N])
{
	int i,j;
	int i1;
	int val1,val2,val3,val4,val5,val6,val7,val8;
	for(i=0;i<64;i+=4){
		for(j=0;j<64;j+=4){
			for(i1=i;i1<i+4;i1+=2){
				val1=A[i1][j+0];
				val2=A[i1][j+1];
				val3=A[i1][j+2];
				val4=A[i1][j+3];
				val5=A[i1+1][j+0];
				val6=A[i1+1][j+1];
				val7=A[i1+1][j+2];
				val8=A[i1+1][j+3];
				B[j+0][i1]=val1;
				B[j+1][i1]=val2;
				B[j+2][i1]=val3;
				B[j+3][i1]=val4;
				B[j+0][i1+1]=val5;
				B[j+1][i1+1]=val6;
				B[j+2][i1+1]=val7;
				B[j+3][i1+1]=val8;
			}
		}
	}
}
/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 
    registerTransFunction(transpose1, transpose1_desc);
    registerTransFunction(transpose2, transpose2_desc);
    registerTransFunction(transpose3, transpose3_desc);
    registerTransFunction(transpose4, transpose4_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

