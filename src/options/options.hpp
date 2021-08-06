#pragma once

#include <iostream>

namespace mipvdb {

enum class mipNamingStyles {
  numbers,
  words
};

inline std::ostream &operator<<(std::ostream &os, const mipNamingStyles &naming) {
  switch (naming) {
    case mipNamingStyles::numbers:
      os << "numbers";
      break;
    case mipNamingStyles::words:
      os << "words";
      break;
  }
  return os;
};

enum class samplingInterpolation {
  point,     //nearest
  box,       //trilinear
  quadratic  //triquadratic
  
};

inline std::ostream &operator<<(std::ostream &os, const samplingInterpolation &sampling) {
  switch (sampling) {
    case samplingInterpolation::point:
      os << "point";
      break;
    case samplingInterpolation::box:
      os << "box";
      break;
    case samplingInterpolation::quadratic:
      os << "quadratic";
      break;
  }
  return os;
};

enum class WordNaming {
  half = 2,
  quater,
  eight,
  sexteenth,
  thirtysecond
};

inline std::ostream &operator<<(std::ostream &os, const WordNaming &level) {
  switch (level) {
    case WordNaming::half:
      os << "half";
      break;
    case WordNaming::quater:
      os << "quater";
      break;
    case WordNaming::eight:
      os << "eight";
      break;
    case WordNaming::sexteenth:
      os << "sixteenth";
      break;
    case WordNaming::thirtysecond:
      os << "thirtysecond";
      break;
  }
  return os;
};

template<typename T> 
std::string enumToString(T enumClass) {
  std::ostringstream oss;
  oss << enumClass;
  return oss.str();
}

struct options {
  bool oneFileMode = false;
  bool verbose = false;
  mipNamingStyles namingStyle = mipNamingStyles::numbers;
  samplingInterpolation intepolation = samplingInterpolation::box;
  std::size_t levels = 2;
  std::string file_in;
  std::string file_out;
};

inline std::ostream &operator<<(std::ostream &os, const options &option) {
  os << "File in: " << option.file_in << "\n";
  os << "File out: " << option.file_out << "\n";
  os << "MIP levels to generate: " << enumToString(option.levels) << "\n";
  os << "Interpolation: " << enumToString(option.intepolation) << "\n";
  os << "Nameing style: " << enumToString(option.namingStyle) << "\n";
  os << "Write all MIP in one vdb file: " << ((option.oneFileMode) ? "On" : "Off") << "\n";
  return os;
};

}  // namespace mipvdb