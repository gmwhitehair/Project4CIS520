#!/bin/bash

# Run make to build the executable
make clean
make

# Create a batch script for sbatch commands (using different number of threads using for loop)s
for i in 1 5 10 20
do
cat <<EOF > mpi_job_script.sh
#!/bin/sh

mpirun -n $i ./mpi_max_ascii_values


EOF

# Give permissions to the job script so it can run
chmod u+x mpi_job_script.sh
#Run the sbatch on the moles utilizing 1G of memory per cpu, and give the correct number of cpus per task, 1cpu/task
    sbatch --constraint=moles --time=02:00:00 --mem-per-cpu=1G --cpus-per-task=1 --ntasks-per-node=$i --nodes=1 ./mpi_job_script.sh
done
