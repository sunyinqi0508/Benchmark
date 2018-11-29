#include <vector>
#include "tools.hpp"
#include <string>
#ifndef DISTRIBUTION
#define DISTRIBUTION
template <typename T> class Distribution {
public:
  virtual std::string num_elems() = 0;
};
#endif
