#include<stdio.h>
#include<mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_Size(MPI_COMM_WORLD, &size);
    MPI_Comm_Rank(MPI_COMM_WORLD, &rank);

    int root=0, tag=0;
    char message[100];
    char messages[][100] = {"HELLO", "CSE", "RVCE"};
    if(rank!=root)
    {
        message = messages[rank-1];
        MPI_Send(message, 100, MPI_CHAR, root, tag, MPI_COMM_WORLD);
    }
    else
    {
        for(int i=1; i<size; i++)
        {
            MPI_Recv(message, 100, MPI_CHAR, i, tag, MPI_COMM_WORLD);
            printf("%s received from process with rank %d\n", message, i);
        }
    }
    MPI_Finalize();

    return 0;
}