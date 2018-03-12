#!/bin/bash
export ROOTPATH="$(dirname "$(readlink -f "$0")")"

usage() {
    echo "Usage:"
    echo -e "\t./run_container.sh [CONTAINER_EXAMPLE_VERSION] [MPI_PROCESSORS_NUMBER]"
    exit 1
}

# Script receives two parameters
# 1: Version of the example to run
# 2: Processor to run in the MPI simulation
if [ "$#" -ne "2" ] ; then
    usage
fi

# Build the example
sudo docker build -f $(pwd)/Dockerfile -t openmpi_example:$1 .

# Run the example
sudo docker run --rm -e PROCESSORS="$2" -it openmpi_example:$1 
