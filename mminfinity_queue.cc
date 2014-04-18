#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <sstream>
#include <cmath>
#include "mminfinity_queue.h"

// compute probability from binomial distr
double factorial(const int n) {
  double f = 1;
  for(int i = 1; i <= n; i++) {
    f *= i;
  }
  return f;
}
double bin(const int k,const int n,const double p) {
  if(k > n) {
    return 0;
  }
  else {
    return factorial(n)/(factorial(k)*factorial(n - k)) * pow(p, k) * pow(1 - p, n - k);
  }
}


void saveArray(const int* array, const int size, std::ofstream *file) {
  for(int i = 0; i < size; i++) {
    *file << array[i] << "\n";
  }
  file->flush();
}

MminfQueue::MminfQueue(const int n, const double mu): n_(n), mu_(mu) {};

void MminfQueue::genTimeFile(const int interval, const int nintervals) {
  std::ofstream times("queue_data/timeData.csv");
  for(int i = 0; i < nintervals+1; i++) {
    times << i*interval << "\n";
  }
  times.flush();
  times.close();
}

void MminfQueue::runQueue(const long int nsteps, const int runID, const int initVal) {
  
  std::stringstream ss;
  ss << "queue_data/yData" << runID << ".csv";
  std::ofstream outputFile(ss.str());
  
  const double dt = 1/((double) n_*n_);
  // also the number of saves
  const int INNER_BLOCK_SIZE = nsteps/OUTER_BLOCK_SIZE;
  double randNums[INNER_BLOCK_SIZE];
  int Y[OUTER_BLOCK_SIZE+1];
  Y[0] = 1;
  int Yt = Y[0];
  
  for(int i = 0; i < OUTER_BLOCK_SIZE; i++) {
    // set up new rng every block
    // usefullness is questionable
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    double rngNormalization = (double) rng.max()+1;
    for(int j = 0; j < INNER_BLOCK_SIZE; j++) {
      randNums[j] = rng()/rngNormalization;
    }
    for(int j = 0; j < INNER_BLOCK_SIZE; j++) {
      if(randNums[j] < 1 - (mu_ + Yt)*dt) {
      }
      else if((randNums[j] >= 1 - (mu_ + Yt)*dt) && (randNums[j] < 1 - mu_*dt)) {
	Yt--;
      }
      else {
	Yt++;
      }
    }
    Y[i+1] = Yt;
  }
  saveArray(Y, OUTER_BLOCK_SIZE+1, &outputFile);
  outputFile.close();
}

void MminfQueue::getDistribution(const double t, const int h, std::vector< double > &pvals, std::vector< double > &kvals) {
  int ESTIMATED_NENTRIES = 10;
  const double cutoff = 0.01;
  double p = 1;
  int k = h;
  // cycle downwards from h
  while(p > cutoff) {
    p = 0;
    for(int i = 0; i < k; i++) {
      double expt = exp(-t);
      p += bin(i, h, expt)*poiss(k - l, (1 - expt)*mu_);
    }
    pvals.push_back(p);
    kvals.push_back(k);
    k--;
  }
  k = h + 1;
  p = 1;
  // cycle upwards from h + 1
  while(p > cutoff) {
    p = 0;
    for(int i = 0; i < k; i++) {
      double expt = exp(-t);
      p += bin(i, h, expt)*poiss(k - l, (1 - expt)*mu_);
    }
    pvals.push_back(p);
    kvals.push_back(k);
    k++;
  }
}
