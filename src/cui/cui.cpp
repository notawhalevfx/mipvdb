#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "../logging/logging.hpp"
#include "cui.hpp"
#include "version.hpp"

namespace po = boost::program_options;

namespace mipvdb {

boost::none_t cui::errorMessage(const std::string &message) {
  logging::errorMessage(message);
  return boost::none;
}

boost::optional<options> cui::arguments(int argc, char *argv[]) {
  options opt;
  try {
    po::options_description generic("Generic options");
    generic.add_options()
      ("version", "Print version")
      ("help,h", "Help message.")
      ("verbose,v", "Verbose mode.");

    po::options_description vdbOptions("VDB MIP options");
    vdbOptions.add_options()
      ("levels,l", po::value<size_t>(&opt.levels), std::string("Numbers of MIP levels to generate. \nDefault: " + std::to_string(opt.levels)).c_str())
      ("interpolation,i", po::value<std::string>(), std::string("Voxel interpolation point, box or quadratic. \nDefault: " + enumToString<samplingInterpolation>(opt.intepolation)).c_str())
      ("naming,n", po::value<std::string>(), std::string("Naming style numbers or words (half, quater, eight, sexteenth, thirtysecond). \nDefault: " + enumToString<mipNamingStyles>(opt.namingStyle)).c_str())
      ("onefile,o", "Write all MIP in one file. \nDefalut: Off");

    po::options_description hidden("Hidden options");
    hidden.add_options()
      ("input-file", po::value<std::string>(), "input vdb file. Required.")
      ("output-file", po::value<std::string>(), "output vdb file. Required.");

    po::positional_options_description pos;
    pos.add("input-file", 1).add("output-file", 1);

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(vdbOptions).add(hidden);

    po::options_description visible_options;
    visible_options.add(generic).add(vdbOptions);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
      .options(cmdline_options).positional(pos).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
      logging::message("Usage: " + std::string(argv[0]) + " [options] in.vdb out.[%l].vdb", true);
      logging::message("%l will be replaced by level in non one file mode.", true);
      logging::message(visible_options, true);
      return boost::none;
    }

    if (vm.count("version")) {
      logging::message(std::string(NAME) + " Version: " + VERSION, true);
      return boost::none;
    }

    if (vm.count("verbose")) {
      logging::enableLogging();
    }

    if (vm.count("onefile")) {
      opt.oneFileMode = true;
    }

    if (vm.count("input-file") && vm.count("output-file")) {
      boost::filesystem::path file_in = vm["input-file"].as<std::string>();
      boost::filesystem::path file_out = vm["output-file"].as<std::string>();
      if (file_in == file_out)
        return errorMessage("Can't be the same file to read and write");
      if (file_in.extension() != ".vdb" || file_out.extension() != ".vdb")
        return errorMessage("Input/Output file should have vdb extension");
      if (!boost::filesystem::exists(file_in))
        return errorMessage("Input file not exist!");
      if (!boost::filesystem::is_regular_file(file_in))
        return errorMessage("Input file is not a file!");
      if (file_out.filename().string().find("%l") == std::string::npos && !opt.oneFileMode)
        return errorMessage("Output file must have %l in name!");

      opt.file_in = file_in.string();
      opt.file_out = file_out.string();
    } else
      return errorMessage("No inputvdb and/or outputvdb.");

    if (vm.count("interpolation")) {
      std::string interp = vm["interpolation"].as<std::string>();
      if (interp == enumToString(samplingInterpolation::point)) {
        opt.intepolation = samplingInterpolation::point;
      } else if (interp == enumToString(samplingInterpolation::box)) {
        opt.intepolation = samplingInterpolation::box;
      } else if (interp == enumToString(samplingInterpolation::quadratic)) {
        opt.intepolation = samplingInterpolation::quadratic;
      } else
        return errorMessage("Wrong interpolation value. Check help!");
    }

    if (vm.count("naming")) {
      std::string naming = vm["naming"].as<std::string>();
      if (naming == enumToString(mipNamingStyles::numbers)) {
        opt.namingStyle = mipNamingStyles::numbers;
      } else if (naming == enumToString(mipNamingStyles::words)) {
        opt.namingStyle = mipNamingStyles::words;
      } else
        return errorMessage("Wrong naming value. Check help!");
    }

    if (vm.count("levels")) {
      opt.levels = vm["levels"].as<size_t>();
    }
  } catch (const boost::program_options::error &ex) {
    return errorMessage(ex.what());
  }

  return opt;
}

}  // namespace mipvdb