## Compile Generated Code

#### Aqua | MPI
To compile the generated code for MPI on Aqua, we require an OpenMPI installation and boost installation, both of which are already available on the system.

Load the linux module named `openmpi411` for OpenMPI version 4.1.1 or `openmpi316` for OpenMPI version 3.1.6. 

```bash
module load openmpi411
```

Once the module has been loaded it should be available at `/lfs/sware/openmpi411/bin/mpic++`.

Boost library on Aqua can be found at `/lfs/usrhome/oth/rnintern/scratch/MPI_Comparison/boost/`.

To access the generated code, navigate to:
`/path/to/StarPlat/graphcode/generated_mpi`. The will be a `.cc` and `.h` file for the algorithm you have generated. You can make a test file (or use the `main.cc` file) that includes the `.cc` file and the necessary headers to run the algorithm on a given graph.

Headers are located one directory tier above where the `.cc` file is executed. As the MPI code generation relies on `boost`, it's necessary to include the respective headers (-I) and link the libraries (-L). Specifically, link against `boost_mpi` and `boost_serialization` since these headers are utilized in the code.

The required `.cc` header files are:
```
|-../mpi_header/graph_mpi.cc
|-../mpi_header/updates.cc
|-../mpi_header/graph_properties/node_property/node_property.cc 
|-../mpi_header/graph_properties/edge_property/edge_property.cc 
|-../mpi_header/rma_datatype/rma_datatype.cc
```

For demonstration purposes, consider the file `sssp_dslV2.cc`.

This is the compilation command for the `sssp_dslV2` algorithm:
```bash
mpicxx -g -std=c++17 \
-o sssp_dslV2 \
-I/lfs/usrhome/oth/rnintern/scratch/MPI_Comparison/boost/install_dir/include \
-L/lfs/usrhome/oth/rnintern/scratch/MPI_Comparison/boost/install_dir/lib \
main.cc sssp_dslV2.cc \
../mpi_header/graph_mpi.cc \
../mpi_header/updates.cc \
../mpi_header/graph_properties/node_property/node_property.cc \
../mpi_header/graph_properties/edge_property/edge_property.cc \
../mpi_header/rma_datatype/rma_datatype.cc
```

#### MacOS | MPI

To compile the generated code for MPI on MacOS, ensure you satisfy the following prerequisites:
- The Boost library is installed on the system.
- The Boost `DYLD_LIBRARY_PATH` (dynamic load library path) should point to `boost/lib`.

Detailed information on these prerequisites can be found in the [Miscellaneous section of the StarPlat Wiki](https://github.com/durwasa-chakraborty/StarPlat/wiki/Miscellaneous#installing-boost-libraries-to-run-starplat-for-mpi).

The generated code resides at:
`/path/to/StarPlat/graphcode/generated_mpi`.
This directory also contains a reference Makefile. It's worth noting that code can be compiled without a `main.cc` file, but to execute the code, a `main` function (or a similar entry point) is essential.

Headers are located one directory tier above where the `.cc` file is executed. As the MPI code generation relies on `boost`, it's necessary to include the respective headers and link the libraries. Specifically, link against `boost_mpi` and `boost_serialization` since these headers are utilized in the code.

For demonstration purposes, consider the file `sssp_dslV2.cc`.

MacOS defaults to using Clang as its compiler. To use `g++` instead, utilize the `OMPI_CXX` environment variable alongside the `mpic++` compiler wrapper. 

The `main.cc` file will need modifications to become the entry point for the algorithm. Include the necessary `.cc` files to link the corresponding `.o` files for generating the `sssp_dslV2` binary.

The required `.cc` files are:
```
|-../mpi_header/graph_mpi.cc
|-../mpi_header/updates.cc
|-../mpi_header/graph_properties/node_property/node_property.cc 
|-../mpi_header/graph_properties/edge_property/edge_property.cc 
|-../mpi_header/rma_datatype/rma_datatype.cc
```

Furthermore, link to the Boost libraries:
```
-I/path/to/boost/include  -L/path/to/boost/lib -lboost_mpi -lboost_serialization
```
Check if the installed `boost` has `include` and `lib`
```bash
if [ -d "/path/to/boost/" ] ; then
    echo "Directory exists."
else
    echo "Directories do not exist."
fi
```

Combining the above details, the complete compilation command becomes:
```
OMPI_CXX=g++ mpic++ -std=c++17 \
-o sssp_dslV2 \
main.cc sssp_dslV2.cc \
../mpi_header/graph_mpi.cc \
../mpi_header/updates.cc \
../mpi_header/graph_properties/node_property/node_property.cc \
../mpi_header/graph_properties/edge_property/edge_property.cc \
../mpi_header/rma_datatype/rma
```

Here's a sample `main.cc` program:

```c++
#include "../mpi_header/graph_mpi.h"
#include "sssp_dslV2.h"

int main(int argc, char *argv[])
{
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;

    Graph graph(argv[1], world);
    world.barrier();

    int src = 0;
    NodeProperty<int> dist;
    Compute_SSSP(graph, dist, graph.weights, 0, world);

    return 0;
}
```

Please note: 
- Please make sure the Boost library is installed correctly and configured like mentioned earlier.
- Adjust the paths in the commands and include statements based on the actual directory structure of your project.