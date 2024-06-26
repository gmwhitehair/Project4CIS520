#!/bin/bash

# Run make to build the executable
make clean
make

# Create a batch script for sbatch commands
cat <<EOF > job_script.sh
#!/bin/sh

# Run the executable
./max_ascii_values

# You can add more commands here if needed

EOF

# Make the batch script executable
chmod u+x job_script.sh

# Submit the batch script using sbatch
#Code gives a different number of cpus per task across different number of threads, also gives more cpu if less total cpus being used
for i in 1 5 10 20
do

    sbatch --constraint=moles --time=02:00:00 --mem-per-cpu=$((20/$i))G --cpus-per-task=$i --ntasks=1 --nodes=1 ./job_script.sh
done
