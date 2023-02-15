#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>

void fun(long long int n, int nthreads)
{
    /*
        This original approach will not yield good results
    */
    // long long int sum = 0, i;
    // double x, y, t = omp_get_wtime();

    // #pragma omp parallel for num_threads(nthreads) private(x,y,i) reduction(+:sum)
    // for(i=0; i<n; i++)
    // {
    //     x = (double)rand()/RAND_MAX;
    //     y = (double)rand()/RAND_MAX;

    //     if((x*x + y*y) < 1)
    //         sum++;
    // }
    // printf("pi:%f time:%lf", (4*sum/(float)n), omp_get_wtime()-t);

    double x, y, t = omp_get_wtime();
    long long int count = 0;
    #pragma omp parallel num_threads(nthreads)
    {
        unsigned int randomState = clock();
        int localCount = 0;
        
        #pragma omp for
        for(long long int i=0; i<n; i++)
        {
            x = (double)rand_r(&randomState)/RAND_MAX;
            y = (double)rand_r(&randomState)/RAND_MAX;
            if((x*x + y*y) < 1)
                localCount++;
        }

        #pragma omp critical
        count += localCount;
    }
    printf("pi:%lf time:%lf", (4*count/(double)n), omp_get_wtime()-t);
}

int main()
{
    for(int t=1; t<=8; t*=2)
    {
        printf("\nFor %d threads", t);
        for(long long int n=10000; n<=10000000; n*=10)
        {
            printf("\nn:%lld ", n);
            fun(n, t);
        }
    }
    printf("\n");

    return 0;
}
