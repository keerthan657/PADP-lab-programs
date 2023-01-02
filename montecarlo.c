#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main()
{
    int n = 10000000;
    int sum=0;
    int i;
    double x,y;

    double t = omp_get_wtime();
    #pragma omp parallel for num_threads(8) private(i,x,y) reduction(+:sum)
    for(i=0; i<n; i++)
    {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;
        if(x*x + y*y < 1)
            sum++;
    }
    printf("value of pi = %lf and time = %lf\n", 4*sum/(double)n, omp_get_wtime()-t);
    return 0;
}