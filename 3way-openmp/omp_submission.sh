#!/bin/bash

# Run make to build the executable
make clean
make

# Create a batch script for sbatch commands


cat <<EOF > omp_job_script.sh
#!/bin/sh

export OMP_NUM_THREADS=1


# Run the executable
./omp_max_ascii_values

# You can add more commands here if needed

EOF

# Make the batch script executable
chmod u+x omp_job_script.sh

# Submit the batch script using sbatch
for i in 1 5 10 20
do

    sbatch --constraint=moles --time=02:00:00 --mem-per-cpu=$((20/$i))G --cpus-per-task=$i --ntasks=1 --nodes=1 ./omp_job_script.sh
done