#include <vector>

#include <openvdb/Grid.h>
#include <openvdb/tools/GridTransformer.h>

#include <tbb/parallel_for.h>
#include <boost/algorithm/string.hpp>

#include "../logging/logging.hpp"

#include "vdb_resize.hpp"

namespace mipvdb {

vdb_resize::vdb_resize(const options &options)
    : _options(options) {
  openvdb::initialize();
}

vdb_resize::~vdb_resize() {
  openvdb::uninitialize();
}

bool vdb_resize::readvdb() {
  logging readTime("File reading");
  openvdb::io::File file(_options.file_in);
  if (!file.open())
    return false;

  for (openvdb::io::File::NameIterator nameIter = file.beginName();
       nameIter != file.endName(); ++nameIter) {
    openvdb::GridBase::Ptr baseGrid = file.readGrid(nameIter.gridName());
    _grids.push_back(baseGrid);
    logging::message("Reading grid: " + baseGrid->getName() + "\tvoxel size: " + baseGrid->transform().voxelSize().str());
  }
  file.close();
  return true;
}

bool vdb_resize::generateMip() {
  if (!readvdb())
    return logging::errorMessage("Something went wrong can't read file");

  for (size_t l = 1; l <= _options.levels; ++l) {
    logging mipGenTime("Generate Mip level: " + std::to_string(l));
    std::vector<openvdb::GridBase::Ptr> grids_out(_grids.size());
    tbb::parallel_for(tbb::blocked_range<size_t>(0, _grids.size()),
      [&](tbb::blocked_range<size_t> r) {
        for (size_t i = r.begin(); i < r.end(); ++i) {
          openvdb::GridBase::Ptr new_grid;
          if (_grids[i]->isType<openvdb::BoolGrid>())
            grids_out[i] = genMip<openvdb::BoolGrid>(openvdb::gridPtrCast<openvdb::BoolGrid>(_grids[i]), l);
          else if (_grids[i]->isType<openvdb::FloatGrid>())
            grids_out[i] = genMip<openvdb::FloatGrid>(openvdb::gridPtrCast<openvdb::FloatGrid>(_grids[i]), l);
          else if (_grids[i]->isType<openvdb::DoubleGrid>())
            grids_out[i] = genMip<openvdb::DoubleGrid>(openvdb::gridPtrCast<openvdb::DoubleGrid>(_grids[i]), l);
          else if (_grids[i]->isType<openvdb::Int32Grid>())
            grids_out[i] = genMip<openvdb::Int32Grid>(openvdb::gridPtrCast<openvdb::Int32Grid>(_grids[i]), l);
          else if (_grids[i]->isType<openvdb::Int64Grid>())
            grids_out[i] = genMip<openvdb::Int64Grid>(openvdb::gridPtrCast<openvdb::Int64Grid>(_grids[i]), l);
          else if (_grids[i]->isType<openvdb::Vec3IGrid>())
            grids_out[i] = genMip<openvdb::Vec3IGrid>(openvdb::gridPtrCast<openvdb::Vec3IGrid>(_grids[i]), l);
          else if (_grids[i]->isType<openvdb::Vec3SGrid>())
            grids_out[i] = genMip<openvdb::Vec3SGrid>(openvdb::gridPtrCast<openvdb::Vec3SGrid>(_grids[i]), l);
          else if (_grids[i]->isType<openvdb::Vec3DGrid>())
            grids_out[i] = genMip<openvdb::Vec3DGrid>(openvdb::gridPtrCast<openvdb::Vec3DGrid>(_grids[i]), l);
          else {
            grids_out[i] = _grids[i];
          }
          logging::message("Generated grid: " + grids_out[i]->getName() + "\ttype: " + grids_out[i]->valueType() +
                            "\tvoxel size: " + grids_out[i]->transform().voxelSize().str());
        }
      });

    std::string path = _options.file_out;
    std::string str_level;
    if (_options.namingStyle == mipNamingStyles::numbers) {
      str_level = std::to_string(l);
    } else {
      str_level = enumToString<WordNaming>(static_cast<WordNaming>(l));
    }
    boost::replace_all(path, "%l", str_level);

    openvdb::io::File(path).write(grids_out);
    logging::message("Saved path: " + path);
  }
  return true;
}

}  // namespace mipvdb