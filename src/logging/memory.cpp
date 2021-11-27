#include <memory>

#include "memory.hpp"

namespace mipvdb {

double memory::getMemoryUsage() {
  struct rusage usage = {};
  int ret = getrusage(RUSAGE_SELF, &usage);
  if (ret == -1)
    return 0;
  constexpr double GbinKb = 1024.0/1024.0;
  return double(usage.ru_maxrss) / GbinKb;
}

}  // namespace mipvdb