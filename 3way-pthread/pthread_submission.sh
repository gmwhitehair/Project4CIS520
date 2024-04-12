#!/bin/bash

# Run make to build the executable
make

# Create a batch script for sbatch commands
cat <<EOF > job_script.sh
#!/bin/sh
hostname

# Run the executable
./max_ascii_values

# You can add more commands here if needed

EOF

# Make the batch script executable
chmod u+x job_script.sh

# Submit the batch script using sbatch
sbatch --time=1 --mem-per-cpu=512M --cpus-per-task=1 --ntasks=1 --nodes=1 job_script.sh
