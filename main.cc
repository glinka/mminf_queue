#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "mminfinity_queue.h"

template <typename T>
void saveVector(const std::vector< T > data, std::ofstream &file) {
  for(typename std::vector< T >::const_iterator val = data.begin(); val != data.end(); val++) {
    file << *val << "\n";
  }
  file.flush();
}

void genTimeFile(const int interval, const int nintervals) {
  std::ofstream times("queue_data/timeData.csv");
  for(int i = 0; i < nintervals+1; i++) {
    times << i*interval << "\n";
  }
  times.flush();
  times.close();
}


int main(int argc, char **argv) {
  // n = 100
  // mu = 0.5 (for complete graph initialization)
   // run for t = 10E8 steps -> T = n^2
  
  // MminfQueue queue(100, 0.5);
  // long int nsteps = 100000000;
  // int nintervals = queue.getOuterBlockSize();
  // std::vector< int > Yvals;
  // Yvals.reserve(nintervals);
  // int interval = nsteps/nintervals;
  // genTimeFile(interval, nintervals);
  // const int nruns = 10;
  // for(int i = 0; i < nruns; i++) {

  //   queue.runQueue(nsteps, Yvals);

  //   std::stringstream ss;
  //   ss << "queue_data/yData" << i << ".csv";
  //   std::ofstream outputFile(ss.str());
  //   saveVector(Yvals, outputFile);
  //   outputFile.close();
  // }
  
  int nthreads = atoi(argv[1]);
  long int tfinal = 1000000000;
  int tinterval = 1000000;
  int nintervals = tfinal / tinterval;
  const int h = 1;
  const int ESTIMATED_N_PVALS = 50;
  // std::vector< double > kvals;
  // std::vector< double > pvals;
  // kvals.reserve(ESTIMATED_N_PVALS);
  // pvals.reserve(ESTIMATED_N_PVALS);
  const int n = 100;
  const double param = 0.5;
  const int nruns = 10;
  long int nsteps = 100000000;
  time_t tstart = clock();
#pragma omp parallel for num_threads(nthreads)
    for(int i = 0; i < nruns; i++) {

      std::vector< int > Yvals;
      Yvals.reserve(ESTIMATED_N_PVALS);
      MminfQueue queue(n, param);
      queue.runQueue(nsteps, Yvals);

      std::stringstream ss;
      ss << "queue_data/yData_" << i << ".csv";
      std::ofstream kDataFile(ss.str());
      saveVector(Yvals, kDataFile);
      kDataFile.close();

    }
    double telapsed = (clock() - tstart)/((double) CLOCKS_PER_SEC);
    std::ofstream timingData("queue_data/tData.csv", std::ios::app);
    timingData << telapsed << "," << nthreads << std::endl;
    timingData.close();
}
