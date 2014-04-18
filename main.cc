#include <cstdlib>
#include "mminfinity_queue.h"

int main(int argc, char **argv) {
  // n = 100
  // mu = 0.5 (for complete graph initialization)
  // run for t = 10E8 steps -> T = n^2
  MminfQueue queue(100, 0.5);
  long int nsteps = 100000000;
  int nintervals = queue.getOuterBlockSize();
  int interval = nsteps/nintervals;
  queue.genTimeFile(interval, nintervals);
  const int n = 10;
  for(int i = 0; i < n; i++) {
    queue.runQueue(nsteps, i);
  }
}
