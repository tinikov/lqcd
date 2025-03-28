#include "tiniqcd.hpp"

#include <cassert>

namespace tiniqcd {

LatticeGeometry::LatticeGeometry()
  : initialized_(false)
  , sites_({ 0 })
  , nodes_({ 0 })
  , nodeSites_({ 0 })
  , nodeCoor_({ -1 })
{
}

void
LatticeGeometry::initialize(int Xsites,
                            int Ysites,
                            int Zsites,
                            int Tsites,
                            int Xnodes,
                            int Ynodes,
                            int Znodes,
                            int Tnodes,
                            MPI_Comm comm)
{
  if (initialized_)
    return;

  // 设置格点参数
  sites_ = { Xsites, Ysites, Zsites, Tsites };
  nodes_ = { Xnodes, Ynodes, Znodes, Tnodes };

  // 计算每个节点的格点数
  for (int i = 0; i < NDIMS; ++i) {
    assert(sites_[i] % nodes_[i] == 0);
    nodeSites_[i] = sites_[i] / nodes_[i];
  }

  // 初始化通信器
  initializeCommunicators(comm);
  initialized_ = true;
}

LatticeGeometry::~LatticeGeometry()
{
  if (initialized_) {
    // 智能指针会自动清理通信器
    initialized_ = false;
  }
}

// MPICommWrapper 实现
MPICommWrapper::MPICommWrapper()
  : comm_(MPI_COMM_NULL)
  , rank_(-1)
  , size_(0)
{
}

MPICommWrapper::~MPICommWrapper()
{
  if (comm_ != MPI_COMM_NULL) {
    MPI_Comm_free(&comm_);
  }
}

void
MPICommWrapper::initialize(MPI_Comm comm)
{
  MPI_Comm_dup(comm, &comm_);
  MPI_Comm_rank(comm_, &rank_);
  MPI_Comm_size(comm_, &size_);
}

} // namespace tiniqcd