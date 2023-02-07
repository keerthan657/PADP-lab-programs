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
```gcc <program_file> something```

(should be -lgd, will confirm)
