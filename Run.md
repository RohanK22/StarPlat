## Run

#### Aqua | MPI

To run MPI programs on Aqua you need to make use of `qsub`. `qsub` is a command used to queue jobs to be run on the cluster. `qsub` takes in a script file as an argument. The script file contains the commands to be run on the cluster.

Here's an example script file:
```bash
#!/bin/bash
#PBS -o logfile.log
#PBS -e errorfile_slash.err
#PBS -l walltime=00:60:00
#PBS -l select=1:mpiprocs=2:ncpus=2
#PBS -q rupesh_gpuq

# Load required module
module load openmpi316

# Set environment variable for PMIX_MCA_gds
export PMIX_MCA_gds=hash

# Ensure the required environment variables are set
if [ -z "$PROB" ] || [ -z "$GRAPH" ] || [ -z "$OUTFILE" ]; then
  echo "One or more required variables are not set. Exiting."
  exit 1
fi

cat "$PBS_NODEFILE"

# Run the MPI job
/lfs/sware/openmpi316/bin/mpirun -hostfile $PBS_NODEFILE "$PROB" "$GRAPH" &> "$OUTFILE"
```


#### MacOS | MPI
1. Open the terminal.
2. Navigate to the directory containing the `sssp_dslV2` binary.
3. Run the following command:
**TODO**: Complete the argument by running a graph end-to-end
```
./sssp_dslV2
```
