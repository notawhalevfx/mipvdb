#include <iostream>

#include "logging/logging.hpp"
#include "options/options.hpp"
#include "cui/cui.hpp"
#include "vdb_resize/vdb_resize.hpp"

int main(int argc, char *argv[]) {
  boost::optional<mipvdb::options> opt = mipvdb::cui::arguments(argc, argv);
  if (!opt)
    return 1;
  mipvdb::logging fullGeneration("Execution time");
  mipvdb::logging::message(*opt);
  mipvdb::vdb_resize vdb_mip(*opt);
  if (!vdb_mip.generateMip())
    return 1;
  return 0;
}