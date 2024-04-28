#!/bin/bash

# Run make to build the executable
make clean
make

# Create a batch script for sbatch commands (using different number of threads using for loop)
for i in 1 5 10 20
do
cat <<EOF > omp_job_script.sh
#!/bin/sh

export OMP_NUM_THREADS=$i


# Run the executable
./omp_max_ascii_values

# You can add more commands here if needed

EOF
# Give permissions to the job script so it can run
chmod u+x omp_job_script.sh
#Run the sbatch on the moles utilizing 1G of memory per cpu, and give the correct number of cpus per task, 1cpu/task
    sbatch --constraint=moles --time=02:00:00 --mem-per-cpu=1G --cpus-per-task=1 --ntasks-per-node=$i --nodes=1 ./omp_job_script.sh
done