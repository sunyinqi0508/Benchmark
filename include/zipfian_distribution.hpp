#ifndef ZIPFIAN_DISTRIBUTION
#define ZIPFIAN_DISTRIBUTION
#include "distribution.hpp"
#include <assert.h>
#include <random>
#include <vector>

template <typename T> class ZipfianDistribution : public Distribution<T> {
private:
  unsigned int n_groups;
  std::uniform_real_distribution<double> uniform;
  std::mt19937 random_engine;
  std::vector<double> proba;
  double zipf_constant;
  static double zeta(int n, double theta) {
    double sum = 0;
    for (long i = 0; i < n; i++) {
      sum += 1 / (pow(i + 1, theta));
    }
    return sum;
  }
  unsigned int nextVal() {
    return static_cast<unsigned int>(
        lower_bound(proba.begin(), proba.end(), uniform(random_engine)) -
        proba.begin());
  }

public:

  void reinit(unsigned int n_groups, std::mt19937 re, double zipf_constant = 1.0){
	assert(n_groups > 1);
	n_groups = n_groups;
	random_engine = re;
	double zetan = 1 / zeta(n_groups, zipf_constant);
	proba.resize(n_groups, 0);
	proba[0] = zetan;
	this->zipf_constant = zipf_constant;
	for (unsigned int i = 1; i < n_groups; ++i)
	  proba[i] = proba[i - 1] + zetan / pow(i + 1, zipf_constant);
  }
  ZipfianDistribution(unsigned int n_groups, std::mt19937 re,
	double zipf_constant = 1.0):uniform{ 0.0, 1.0 } {
	reinit(n_groups, re, zipf_constant);
  }

  unsigned int* get_rawdata(int n_elements) {
	unsigned int *ret = new unsigned int[n_elements];
	for (int i = 0; i < n_elements; i++)
	  ret[i] = nextVal();
	return ret;
  }
  std::string num_elems() { return std::to_string(n_groups); }
};
#endif
