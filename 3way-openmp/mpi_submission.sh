#!/bin/bash

# Run make to build the executable
make clean
make

# Create a batch script for sbatch commands


# Submit the batch script using sbatch
for i in 1 5 10 20
do
cat <<EOF > mpi_job_script.sh
#!/bin/sh

mpirun -n $i ./mpi_max_ascii_values


EOF

chmod u+x mpi_job_script.sh
    sbatch --constraint=moles --time=02:00:00 --mem-per-cpu=1G --cpus-per-task=1 --ntasks-per-node=$i --nodes=1 ./mpi_job_script.sh
done