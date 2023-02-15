#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long int n = 1000000;
    int localCount = 0, count = 0;
    double x, y;
    for(long long int i=0; i<n; i++)
    {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;
        if((x*x + y*y) < 1)
            localCount++;
    }

    MPI_Reduce(&localCount, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank==0)
        printf("value of pi: %lf\n", (4*count/(double)(n*size)));

    MPI_Finalize();

    return 0;
}
