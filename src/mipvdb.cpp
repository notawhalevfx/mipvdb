#include <vector>

#include "logging/logging.hpp"
#include "options/options.hpp"
#include "cli/cli.hpp"
#include "vdb_resize/vdb_resize.hpp"

int main(int argc, char *argv[]) {
  const std::vector<std::string> arg(argv + 1, argv + argc);
  boost::optional<mipvdb::options> opt = mipvdb::cli::arguments(arg);
  if (!opt)
    return 1;
  mipvdb::logging fullGeneration("Execution time");
  mipvdb::logging::message(*opt);
  mipvdb::vdb_resize vdb_mip(*opt);
  if (!vdb_mip.generateMip())
    return 1;
  return 0;
}