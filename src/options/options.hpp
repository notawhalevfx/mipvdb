#pragma once

#include <iostream>
#include <sstream>

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
  half = 1,
  quater,
  eight,
  sexteenth,
  thirtysecond,
  sixtyfourth
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
    case WordNaming::sixtyfourth:
      os << "sixtyfourth";
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

inline std::string namingSuffix(const size_t &level, const options &opt) {
  std::string str_level;
  if (opt.namingStyle == mipNamingStyles::numbers) {
    str_level = std::to_string(level);
  } else {
    str_level = enumToString<WordNaming>(static_cast<WordNaming>(level));
  }
  return std::move(str_level);
}


}  // namespace mipvdb