#include<stdio.h>
#include<stdlib.h>
#include<math.h>    // use -lm compiling flag
#include<omp.h>
#include<string.h>

#define true 1
#define false 0

long long int minimum(long long int x, long long int y)
{
    if(x<y)
        return x;
    else
        return y;
}

void setArray(int* arr, int val, long long int size)
{
    for(long long int i=2; i<=size; i++)
        arr[i] = val;
}

long long int strike(int *composite, long long int i, long long int stride, long long int limit)
{
    // for multiples starting from i*i, mark them
    for(; i<=limit; i+=stride)
        composite[i] = true;
    
    return i;
}

void cache_unfriendly_sieve(long long int n)
{
    int cnt=0;
    long long int sqrt_n = (long long int)sqrt(n);

    // initialize composite array to false
    int* composite = malloc((n+1)*sizeof(int));
    setArray(composite, false, n);

    double t = omp_get_wtime();
    // for each prime encountered, eliminate its multiples
    for(long long int i=2; i<=sqrt_n; i++)
    {
        // if not composite (i.e. prime), strike out all its multiples
        if(!composite[i])
        {
            strike(composite, 2*i, i, n);
            cnt++;
        }
    }
    for(long long int i=sqrt_n+1; i<=n; i++)
    {
        if(!composite[i])
            cnt++;
    }

    free(composite);

    printf("CACHE UNFRIENDLY SIEVE - %d primes found, time taken=%lf\n", cnt, omp_get_wtime()-t);
}

void cache_friendly_sieve(long long int n)
{
    int cnt=0;
    long long int sqrt_n = (long long int)sqrt(n);

    // initialize composite array to false
    int* composite = malloc((n+1)*sizeof(int));
    setArray(composite, false, n);
    // create new arrays - factor & striker - with size sqrt_n
    long long int* factor  = malloc(sqrt_n*sizeof(int));
    long long int* striker = malloc(sqrt_n*sizeof(int));
    long long int n_factor = 0;

    double t = omp_get_wtime();
    // for each prime encountered, eliminate its multiples
    for(long long int i=2; i<=sqrt_n; i++)
    {
        // if not composite (i.e. prime), strike out all its multiples
        if(!composite[i])
        {
            striker[n_factor] = strike(composite, 2*i, i, sqrt_n);
            factor[n_factor] = i;
            n_factor++;
            cnt++;
        }
    }

    // chop all elements into windows of size sqrt_n
    for(long long int window=sqrt_n+1; window<=n; window+=sqrt_n)
    {
        long long int limit = minimum(window+sqrt_n-1, n);
        for(long long int k=0; k<n_factor; k++)
            striker[k] = strike(composite, striker[k], factor[k], limit);
        
        for(long long int i=window; i<=limit; i++)
            if(!composite[i])
                cnt++;
    }

    free(composite);
    free(factor);
    free(striker);

    printf("CACHE   FRIENDLY SIEVE - %d primes found, time taken=%lf\n", cnt, omp_get_wtime()-t);
}

void parallel_sieve(long long int n, int numThreads)
{
    int cnt=0;
    long long int sqrt_n = (long long int)sqrt(n);

    long long int* factor = malloc(sqrt_n*sizeof(int));
    long long int n_factor = 0;

    double t = omp_get_wtime();
    omp_set_num_threads(numThreads);

    #pragma omp parallel
    {
        int* composite = malloc((n+1)*sizeof(int));
        long long int* striker = malloc(sqrt_n*sizeof(long long int));

        #pragma omp single
        {
            setArray(composite, false, sqrt_n);
            for(long long int i=2; i<=sqrt_n; i++)
            {
                if(!composite[i])
                {
                    striker[n_factor] = strike(composite, 2*i, i, sqrt_n);
                    factor[n_factor] = i;
                    n_factor++;
                    cnt++;
                }
            }
        }

        long long int base = -1;
        #pragma omp for reduction(+:cnt)
        for(long long int window=sqrt_n+1; window<=n; window+=sqrt_n)
        {
            setArray(composite, false, sqrt_n);
            if(base!=window)
            {
                // compute striker from scratch
                base = window;
                for(long long int k=0; k<n_factor; k++)
                    striker[k] = (base+factor[k]-1)/factor[k] * factor[k] - base;
            }

            long long int limit = minimum(window+sqrt_n-1, n) - base;
            for(long long int k=0; k<n_factor; k++)
                striker[k] = strike(composite, striker[k], factor[k], limit) - sqrt_n;
            
            for(long long int i=0; i<=limit; i++)
                if(!composite[i])
                cnt++;
            
            base += sqrt_n;
        }
        free(composite);
        free(striker);
    }

    free(factor);

    printf("CACHE   PARALLEL SIEVE - %d primes found, time taken=%lf\n", cnt, omp_get_wtime()-t);
}

int main()
{
    long long int n = 1000000;

    cache_unfriendly_sieve(n);

    cache_friendly_sieve(n);

    parallel_sieve(n, 16);

    return 0;
}