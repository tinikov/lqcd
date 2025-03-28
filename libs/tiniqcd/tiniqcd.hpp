#pragma once

#include <mpi.h>

#include <array>
#include <memory>
#include <stdexcept>

namespace tiniqcd {

// 前向声明
class MPICommWrapper;

class LatticeGeometry
{
public:
  // 构造函数和析构函数
  LatticeGeometry();
  ~LatticeGeometry();

  // 禁用拷贝
  LatticeGeometry(const LatticeGeometry&) = delete;
  LatticeGeometry& operator=(const LatticeGeometry&) = delete;

  // 初始化方法
  void initialize(int Xsites,
                  int Ysites,
                  int Zsites,
                  int Tsites,
                  int Xnodes,
                  int Ynodes,
                  int Znodes,
                  int Tnodes,
                  MPI_Comm comm);

  // 获取格点信息的方法
  int getSites(int dim) const { return sites_[dim]; }
  int getNodes(int dim) const { return nodes_[dim]; }
  int getNodeSites(int dim) const { return nodeSites_[dim]; }
  int getNodeCoor(int dim) const { return nodeCoor_[dim]; }

  // 获取通信器
  // MPI_Comm getComm4D() const;
  // MPI_Comm getComm3D() const;
  // MPI_Comm getComm1D(int dim) const;

private:
  static constexpr int NDIMS = 4;
  bool initialized_;

  std::array<int, NDIMS> sites_;     // X,Y,Z,T sites
  std::array<int, NDIMS> nodes_;     // X,Y,Z,T nodes
  std::array<int, NDIMS> nodeSites_; // sites per node
  std::array<int, NDIMS> nodeCoor_;  // node coordinates

  // 使用智能指针管理MPI通信器
  std::unique_ptr<MPICommWrapper> comm4D_;
  std::unique_ptr<MPICommWrapper> comm3D_;
  std::array<std::unique_ptr<MPICommWrapper>, NDIMS> comm1D_;

  // void initializeDimensions();
  void initializeCommunicators(MPI_Comm baseComm);
};

// MPI通信器包装类
class MPICommWrapper
{
public:
  explicit MPICommWrapper();
  ~MPICommWrapper();

  void initialize(MPI_Comm comm);
  MPI_Comm get() const { return comm_; }
  int getRank() const { return rank_; }
  int getSize() const { return size_; }

private:
  MPI_Comm comm_;
  int rank_;
  int size_;
};

} // namespace tiniqcd