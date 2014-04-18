#ifndef MMINF_QUEUE_H
#define MMINF_QUEUE_H
#include <vector>

class MminfQueue {
 private:
  const int n_;
  const double mu_;
  const int OUTER_BLOCK_SIZE = 100000;
 public:
  void getDistribution(const double t, const int h, std::vector< double > &pvals, std::vector< double > &kvals);
  void runQueue(const long int nsteps, std::vector< int > &Yvals, const int initVal = 1);
  int getOuterBlockSize() {
    return OUTER_BLOCK_SIZE;
  }
  MminfQueue(const int n, const double mu);
  ~MminfQueue() {};
};

#endif
