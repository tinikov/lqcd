#include "tiniqcd.hpp"
#include <mpi.h>

int
main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  tiniqcd::LatticeGeometry lattice;

  // 初始化格点
  lattice.initialize(32,
                     32,
                     32,
                     64, // 格点大小
                     2,
                     2,
                     2,
                     4,               // 节点分布
                     MPI_COMM_WORLD); // 基础通信器

  // 使用格点信息
  int xSites = lattice.getSites(0);
  // MPI_Comm comm4D = lattice.getComm4D();

  MPI_Finalize();
  return 0;
}