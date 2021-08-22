#pragma once

#include <chrono>
#include <iostream>
#include <mutex>
#include <string>

namespace mipvdb {

class logging {
 public:
  logging(const std::string &message);
  ~logging();

  static inline void enableLogging() { _verbose = true; };

  template <typename T>
  static inline bool errorMessage(const T &message) {
    std::lock_guard<std::mutex> lock(_mutex);
    std::cerr << message << "\n";
    return false;
  }

  template <typename T>
  static inline bool message(const T &message, const bool force = false) {
    if (_verbose || force) {
      std::lock_guard<std::mutex> lock(_mutex);
      std::cout << message << "\n";
    }
    return true;
  };

 private:
  const std::chrono::high_resolution_clock::time_point _startTime;
  const std::string _message;
  static bool _verbose;
  static std::mutex _mutex;
};

}  // namespace mipvdb