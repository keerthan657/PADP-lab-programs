#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char messages[][100] = {"HELLO", "CSE", "RVCE"};
    char buffer[100];

    if(rank==0)
    {
        // receive from other processes
        for(int i=1; i<size; i++)
        {
            MPI_Recv(buffer, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("\nreceived %s from process %d", buffer, i);
        }
    }
    else
    {
        // send to rank-0 process
        MPI_Send(messages[rank-1], 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    printf("\n");
    return 0;
}
