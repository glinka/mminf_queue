#include <random>
#include <chrono>
#include <cmath>
#include "mminfinity_queue.h"

double factorial(const int n) {
  double f = 1;
  for(int i = 1; i <= n; i++) {
    f *= i;
  }
  return f;
}

// compute probability from binomial distr
double bin(const int k,const int n,const double p) {
  if(k > n) {
    return 0;
  }
  else {
    return factorial(n)/(factorial(k)*factorial(n - k)) * pow(p, k) * pow(1 - p, n - k);
  }
}

// compute probability from poisson distr
double poiss(const int k, const double l) {
  return exp(-l)*pow(l, k)/factorial(k);
}

MminfQueue::MminfQueue(const int n, const double mu): n_(n), mu_(mu) {};

void MminfQueue::runQueue(const long int nsteps, std::vector< int > &Yvals, const int initVal) {
  
  const double dt = 1/((double) n_*n_);
  // also the number of saves
  const int INNER_BLOCK_SIZE = nsteps/OUTER_BLOCK_SIZE;
  double randNums[INNER_BLOCK_SIZE];
  Yvals.push_back(1);
  int Yt = Yvals[0];
  
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
    Yvals.push_back(Yt);
  }
}

void MminfQueue::getDistribution(const double t, const int h, std::vector< double > &pvals, std::vector< double > &kvals) {
  const double cutoff = 0.01;
  double p = 1;
  int k = h;
  // cycle downwards from h
  while(p > cutoff) {
    p = 0;
    for(int i = 0; i <= k; i++) {
      double expt = exp(-t);
      p += bin(i, h, expt)*poiss(k - i, (1 - expt)*mu_);
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
    for(int i = 0; i <= k; i++) {
      double expt = exp(-t);
      p += bin(i, h, expt)*poiss(k - i, (1 - expt)*mu_);
    }
    pvals.push_back(p);
    kvals.push_back(k);
    k++;
  }
}
