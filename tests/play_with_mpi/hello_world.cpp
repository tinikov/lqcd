#include<mpi.h>
#include<iostream>

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::cout << "Hello, world! I am process " << rank << " of " << size << std::endl;

  MPI_Finalize();
  return 0;
}