#pragma once

#include <sys/resource.h>

namespace mipvdb {

class memory {
 public:
  memory() = delete;
  ~memory() = delete;
  static double getMemoryUsage();
};

}  // namespace mipvdb