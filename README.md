# PADP-lab-programs

## Programs list

1. (a) Monte-Carlo
   (b) Monte-Carlo using MPI

2. Matrix Multiplication

3. Sieve of Eratosthenes

4. Image conversion

5. MPI hello world

6. Word search

7. SAXPY using OpenCL

## Commands

### Normal OpenMP program
```gcc <program_file> -fopenmp```

### For MPI programs
```mpicc <program_file>```

```mpirun ./a.out -np 4```

(4 represents 4 processes)

### For image conversion
```gcc <program_file> -lgd -fopenmp```

### If used <math.h>, add ```-lm``` compiler flag while compiling

## IMPORTANT
The previous codes were just written and not run on any machine. It has been updated now with proper codes.
Unfortunately, the previous codes had some mistakes.
Here is the list of changes that were made:
1. In montecarlo, updated with rand_r() as per [mahesh's blog](https://mahesh-hegde.github.io/posts/omp_rand_critical_section/)
2. In montecarlo MPI, in MPI_Reduce, 0 and 1 should be exchanged
```MPI_Reduce(&localCount, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD)```
3. No changes in matrix multiplication
4. In image program, the variable ```gdImagePtr *img```. Note that img should not be a pointer. Replaced with ```gdImagePtr img```. Also small typo with gdImageRed.
5. In MPI hello world, MPI_Recv will have an extra argument at the end, add MPI_STATUS_IGNORE
```MPI_Recv(buffer, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)```
6. Word search should be good, but i am not sure where to put #pragma omp primitive. Reference other links for this.
