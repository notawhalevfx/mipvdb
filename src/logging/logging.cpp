#include <iomanip>
#include <iostream>
#include <sstream>

#include "logging.hpp"
#include "memory.hpp"

namespace mipvdb {

bool logging::_verbose = false;
std::mutex logging::_mutex;

logging::logging(const std::string &message)
    : _startTime(std::chrono::high_resolution_clock::now()),
      _message(message) {}

logging::~logging() {
  std::lock_guard<std::mutex> lock(_mutex);
  auto per = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - _startTime);
  std::cout << std::fixed << std::setprecision(4);
  std::cout << "[";
  std::cout << per.count() << "s " << std::setprecision(2) << memory::getMemoryUsage() << "Gb";
  std::cout << "] ";
  std::cout << std::resetiosflags(std::ios::showbase);
  std::cout << _message << "\n";
}

}  // namespace mipvdb