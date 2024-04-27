All three of our implementations sit within respective folders. Here are the commands to run the code on Beocat. 

Right now the number of lines read in from the file is set at 1,000,000 for each implementation.

For PThreads the number of threads is set at 20. It can be altered under the LINE_COUNT variable at the top of main.c

MPI and OpenMP iterate through 1, 5, 10, and 20 threads. This cannot be altered.

COMMANDS TO RUN:

From the h4 directory - 

PThreads:
cd 3way-pthread; ./pthread_submission.sh; kstat -d 1

MPI:
cd 3way-mpi; ./mpi_submission.sh; kstat -d 1

Open MP:
cd 3way-openmp; ./omp_submission.sh; kstat -d 1


