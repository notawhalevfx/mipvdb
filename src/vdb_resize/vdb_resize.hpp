#pragma once

#include <openvdb/openvdb.h>
#include <openvdb/tools/GridTransformer.h>

#include "../options/options.hpp"

namespace mipvdb {

class vdb_resize {
 public:
  explicit vdb_resize(const options &options);
  ~vdb_resize();

  bool generateMip();

 private:
  bool readVdb();
  void writeVdb(const std::string &writefile, const openvdb::GridPtrVec &gridWrites);
  openvdb::GridBase::Ptr genMip(const openvdb::GridBase::Ptr& grid, const size_t &level);

  template <typename GridType>
  inline openvdb::GridBase::Ptr genMipExec(const typename GridType::Ptr &inGrid, const size_t &level) {
    typename GridType::Ptr outGrid = GridType::create();
    outGrid->setName(inGrid->getName());
    outGrid->setGridClass(inGrid->getGridClass());

    openvdb::math::Transform::Ptr xform = inGrid->transform().copy();
    xform->preScale(pow(2,level));
    outGrid->setTransform(xform);

    const openvdb::math::Transform &sourceXform = inGrid->transform();
    const openvdb::math::Transform &targetXform = outGrid->transform();
    openvdb::Mat4R xform2 =
        sourceXform.baseMap()->getAffineMap()->getMat4() *
        targetXform.baseMap()->getAffineMap()->getMat4().inverse();
    // Create the transformer.
    openvdb::tools::GridTransformer transformer(xform2);

    switch (_options.intepolation) {
      case samplingInterpolation::point:
        transformer.transformGrid<openvdb::tools::PointSampler, GridType>(*inGrid, *outGrid);
        break;
      case samplingInterpolation::box:
        transformer.transformGrid<openvdb::tools::BoxSampler, GridType>(*inGrid, *outGrid);
        break;
      case samplingInterpolation::quadratic:
        transformer.transformGrid<openvdb::tools::QuadraticSampler, GridType>(*inGrid, *outGrid);
        break;
    }
    return outGrid;
  }

 private:
  const options &_options;
  openvdb::GridPtrVec _grids;
};

}  // namespace mipvdb