#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main()
{
    int n = 1000;
    int i,j,k;

    int *A = malloc((n*n)*sizeof(int));
    int *B = malloc((n*n)*sizeof(int));
    int *C = malloc((n*n)*sizeof(int));

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
        {
            A[i*n + j] = i*j;
            B[i*n + j] = i+j;
            C[i*n + j] = 0;
        }
    
    double t = omp_get_wtime();
    #pragma omp parallel for num_threads(8) private(i,j,k)
    for(i=0; i<n; i++)
        for(j=0; j<n; j++)
            for(k=0; k<n; k++)
                C[i*n + j] += A[i*n + k] * B[k*n + j];
    
    printf("time taken = %lf\n", omp_get_wtime()-t);
    return 0;
}