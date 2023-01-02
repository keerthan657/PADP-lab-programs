#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_Rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_Size(MPI_COMM_WORLD, &size);

    int sum=0, count=0, n=1000000;
    for(int i=0; i<n; i++)
    {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;
        if(x*x + y*y < 1)
            count++;
    }

    MPI_Reduce(&count, &sum, 0, MPI_INT, MPI_SUM, 1, MPI_COMM_WORLD);

    printf("value of pi=%lf\n", 4*sum/(double)n);
    return 0;
}