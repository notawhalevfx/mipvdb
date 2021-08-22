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

bool vdb_resize::readVdb() {
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

void vdb_resize::writeVdb(const std::string &writefile, const openvdb::GridPtrVec &gridWrites) {
  logging savingTime("Saved path: " + writefile);
  openvdb::io::File(writefile).write(gridWrites);
}

openvdb::GridBase::Ptr vdb_resize::genMip(const openvdb::GridBase::Ptr& grid, const size_t &level) {
  if (grid->isType<openvdb::BoolGrid>())
    return genMipExec<openvdb::BoolGrid>(openvdb::gridPtrCast<openvdb::BoolGrid>(grid), level);
  else if (grid->isType<openvdb::FloatGrid>())
    return genMipExec<openvdb::FloatGrid>(openvdb::gridPtrCast<openvdb::FloatGrid>(grid), level);
  else if (grid->isType<openvdb::DoubleGrid>())
    return genMipExec<openvdb::DoubleGrid>(openvdb::gridPtrCast<openvdb::DoubleGrid>(grid), level);
  else if (grid->isType<openvdb::Int32Grid>())
    return genMipExec<openvdb::Int32Grid>(openvdb::gridPtrCast<openvdb::Int32Grid>(grid), level);
  else if (grid->isType<openvdb::Int64Grid>())
    return genMipExec<openvdb::Int64Grid>(openvdb::gridPtrCast<openvdb::Int64Grid>(grid), level);
  else if (grid->isType<openvdb::Vec3IGrid>())
    return genMipExec<openvdb::Vec3IGrid>(openvdb::gridPtrCast<openvdb::Vec3IGrid>(grid), level);
  else if (grid->isType<openvdb::Vec3SGrid>())
    return genMipExec<openvdb::Vec3SGrid>(openvdb::gridPtrCast<openvdb::Vec3SGrid>(grid), level);
  else if (grid->isType<openvdb::Vec3DGrid>())
    return genMipExec<openvdb::Vec3DGrid>(openvdb::gridPtrCast<openvdb::Vec3DGrid>(grid), level);
  else {
    return grid;
  }
}


bool vdb_resize::generateMip() {
  if (!readVdb())
    return logging::errorMessage("Something went wrong can't read file");

  if (!_options.oneFileMode) {
    for (size_t l = 1; l <= _options.levels; ++l) {
      logging mipGenTime("Generate Mip level: " + std::to_string(l));
      openvdb::GridPtrVec grids_out(_grids.size());
      tbb::parallel_for(tbb::blocked_range<size_t>(0, _grids.size()),
        [&](tbb::blocked_range<size_t> r) {
          for (size_t i = r.begin(); i < r.end(); ++i) {
            grids_out[i] = genMip(_grids[i],l);
            logging::message("Generated grid: " + grids_out[i]->getName() + "\ttype: " + grids_out[i]->valueType() +
                             "\tvoxel size: " + grids_out[i]->transform().voxelSize().str());
          }
        });

      std::string path = _options.file_out;
      boost::replace_all(path, "%l", namingSuffix(l,_options));

      writeVdb(path,grids_out);
    }
  } else {
    openvdb::GridPtrVec grids_out = _grids;
    grids_out.resize(_grids.size()*(_options.levels+1));
      tbb::parallel_for(tbb::blocked_range<size_t>(_grids.size(), _grids.size()*(_options.levels+1)),
        [&](tbb::blocked_range<size_t> r) {
          for (size_t i = r.begin(); i < r.end(); ++i) {
            size_t l = i/_grids.size();
            size_t index = i%_grids.size();
            grids_out[i] = genMip(_grids[index],l);
            grids_out[i]->setName(grids_out[i]->getName()+"_"+namingSuffix(l,_options));
            logging::message("Generated grid: " + grids_out[i]->getName() + "\ttype: " + grids_out[i]->valueType() +
                             "\tvoxel size: " + grids_out[i]->transform().voxelSize().str());
          }
      });
    writeVdb(_options.file_out,grids_out);
  }
  return true;
}

}  // namespace mipvdb