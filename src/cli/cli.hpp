#pragma once

#include <boost/optional.hpp>

#include "../options/options.hpp"

namespace mipvdb {

class cli {
 public:
  cli() = delete;
  ~cli() = delete;

  static boost::optional<options> arguments(int argc, char *argv[]);
  static boost::none_t errorMessage(const std::string &message);

};

}  // namespace mipvdb