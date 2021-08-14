#include <iostream>
#include <sstream>
#include <iomanip>

#include "logging.hpp"

namespace mipvdb {

bool logging::_verbose = false;

logging::logging(const std::string &message)
    : _startTime(std::chrono::high_resolution_clock::now()),
      _message(message) {}

logging::~logging() {
  auto per = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - _startTime);
  std::cout << std::setprecision(4) << "[" << per.count() << " s] " << _message << "\n";
}

}  // namespace mipvdb