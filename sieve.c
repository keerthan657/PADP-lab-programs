#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>

#define true 1
#define false 0
#define llint long long int

llint minimum(llint x, llint y)
{
    if(x<y)
        return x;
    else
        return y;
}

void setArray(int *arr, int val, llint n)
{
    for(llint i=0; i<n; i++)
        *(arr+i) = val;
}

llint strike(int *composite, llint i, llint stride, llint limit)
{
    for(;i<=limit; i+=stride)
        *(composite+i) = true;
    
    return i;
}

void cacheUnfriendlySieve(llint n)
{
    int cnt = 0;
    llint m = (llint)sqrt(n);

    int *composite = (int *)malloc((n+1)*sizeof(int));
    setArray(composite, false, n);

    double t = omp_get_wtime();
    for(llint i=2; i<=m; i++)
    {
        if(!composite[i])
        {
            strike(composite, 2*i, i, n);
            cnt++;
        }
    }
    for(llint i=m+1; i<=n; i++)
    {
        if(!composite[i])
            cnt++;
    }
    
    printf("\nCUS: count=%d, time=%lf", cnt, omp_get_wtime()-t);

    free(composite);
}

void cacheFriendlySieve(llint n)
{
    int cnt=0;
    llint m = (llint)sqrt(n);

    int *composite = (int *)malloc((n+1)*sizeof(int));
    setArray(composite, false, n);

    llint *factor = (llint *)malloc(m*sizeof(llint));
    llint *striker = (llint *)malloc(m*sizeof(llint));
    llint n_factor = 0;

    double t = omp_get_wtime();
    for(llint i=2; i<=m; i++)
    {
        if(!composite[i])
        {
            striker[n_factor] = strike(composite, 2*i, i, m);
            factor[n_factor] = i;
            n_factor++;
            cnt++;
        }
    }

    for(llint window=m+1; window<=n; window+=m)
    {
        llint limit = minimum(window+m-1, n);
        for(llint k=0; k<n_factor; k++)
            striker[k] = strike(composite, striker[k], factor[k], limit);
        
        for(llint i=window; i<=limit; i++)
            if(!composite[i])
                cnt++;
    }

    printf("\nCFS: count=%d, time=%lf", cnt, omp_get_wtime()-t);

    free(composite);
    free(factor);
    free(striker);
}

void cacheParallelSieve(llint n)
{
    int cnt = 0;
    llint m = (llint)sqrt(n);
    
    llint *factor = (llint *)malloc(m*sizeof(llint));
    llint n_factor = 0;

    double t = omp_get_wtime();
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        int *composite = (int *)malloc((n+1)*sizeof(int));
        llint *striker = (llint *)malloc(m*sizeof(llint));

        #pragma omp single
        {
            setArray(composite, false, m);// mn
            for(llint i=2; i<=m; i++)
            {
                if(!composite[i])
                {
                    striker[n_factor] = strike(composite, 2*i, i, m);
                    factor[n_factor] = i;
                    n_factor++;
                    cnt++;
                }
            }
        }

        llint base = -1;
        #pragma omp for reduction(+:cnt)
        for(llint window=m+1; window<=n; window+=m)
        {
            setArray(composite, false, m);
            if(base!=window)
            {
                base = window;
                for(llint k=0; k<n_factor; k++)
                    striker[k] = (base+factor[k]-1)/factor[k] * factor[k] - base;
            }

            llint limit = minimum(window+m-1, n) - base;
            for(llint k=0; k<n_factor; k++)
                striker[k] = strike(composite, striker[k], factor[k], limit) - m;
            for(llint i=0; i<=limit; i++)
                if(!composite[i])
                cnt++;
            
            base += m;
        }
        free(composite);
        free(striker);
    }
    free(factor);

    printf("\nCPS: count=%d, time=%lf", cnt, omp_get_wtime()-t);
}

int main()
{
    // give a very large value, else graph will not be proper
    llint n = 100000000;

    cacheUnfriendlySieve(n);

    cacheFriendlySieve(n);

    cacheParallelSieve(n);

    printf("\n");
    return 0;
}
