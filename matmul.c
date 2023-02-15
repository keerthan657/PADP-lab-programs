#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

void fun(long long int n, int nthreads)
{
    double t = omp_get_wtime();

    // allocate memory
    int *A = (int*)malloc(n*n*sizeof(int));
    int *B = (int*)malloc(n*n*sizeof(int));
    int *C = (int*)malloc(n*n*sizeof(int));

    // initialize with random values
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
        {
            A[i*n + j] = i+j;
            B[i*n + j] = i-j;
            C[i*n + j] = 0;
        }

    // perform multiplication
    int i,j,k;

    #pragma omp parallel for num_threads(nthreads) private(i,j,k)
    for(i=0; i<n; i++)
        for(j=0; j<n; j++)
            for(k=0; k<n; k++)
                C[i*n + j] += A[i*n + k] + B[k*n + j];
    
    printf("time:%lf", omp_get_wtime()-t);
}

int main()
{
    for(int t=1; t<=8; t*=2)
    {
        printf("\nFor %d threads", t);
        for(long long int n=1000; n<=1200; n+=200)
        {
            printf("\nn:%lld ", n);
            fun(n,t);
        }
    }
    printf("\n");

    return 0;
}
