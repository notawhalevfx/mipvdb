#pragma once

#include <vector>

#include <boost/optional.hpp>

#include "../options/options.hpp"

namespace mipvdb {

class cli {
 public:
  cli() = delete;
  ~cli() = delete;

  static boost::optional<options> arguments(const std::vector<std::string> arg);
  static boost::none_t errorMessage(const std::string &message);

};

}  // namespace mipvdb