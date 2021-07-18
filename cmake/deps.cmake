include(ExternalProject)

# blosc
ExternalProject_Add(blosc
  PREFIX ${CMAKE_BINARY_DIR}/deps
  GIT_REPOSITORY git@github.com:Blosc/c-blosc.git
  GIT_TAG v1.5.0
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR} -DBUILD_STATIC=ON
)

include(ProcessorCount)
ProcessorCount(N)
message("Found ${N} Threads")

# tbb
# TODO in future test static library
ExternalProject_Add(tbb
  PREFIX ${CMAKE_BINARY_DIR}/deps
  GIT_REPOSITORY git@github.com:oneapi-src/oneTBB.git
  GIT_TAG 2018_U6
  CONFIGURE_COMMAND ""
  BUILD_IN_SOURCE ON
  BUILD_COMMAND make -j${N} tbb tbbmalloc tbbproxy
  INSTALL_COMMAND sh -c "cp -u -v -R ${CMAKE_BINARY_DIR}/deps/src/tbb/build/linux*/*.so* ${CMAKE_BINARY_DIR}/lib/" &&
  sh -c "cp -u -v -R ${CMAKE_BINARY_DIR}/deps/src/tbb/include/tbb ${CMAKE_BINARY_DIR}/include/"
)

# TODO: Add jemalloc in future release
# jemalloc
# ExternalProject_Add(jemalloc
#   GIT_REPOSITORY git@github.com:jemalloc/jemalloc.git
#   GIT_TAG 5.2.1
#   CONFIGURE_COMMAND ""
#   BUILD_IN_SOURCE ON
# #   BUILD_COMMAND ./congfigure make -j${N}
# #   INSTALL_COMMAND ""
# #   INSTALL_COMMAND sh -c "cp -u -v -R ${CMAKE_BINARY_DIR}/tbb-prefix/src/tbb/build/linux*/*.so ${CMAKE_BINARY_DIR}/lib/" &&
# #   sh -c "cp -u -v -R ${CMAKE_BINARY_DIR}/tbb-prefix/src/tbb/include/tbb ${CMAKE_BINARY_DIR}/include/"
# )

# OpenVDB
ExternalProject_Add(openvdb
  DEPENDS tbb blosc
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/deps
  GIT_REPOSITORY git@github.com:AcademySoftwareFoundation/openvdb.git
  GIT_TAG v8.1.0
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
  -DTBB_ROOT=${CMAKE_BINARY_DIR}
  -DBlosc_ROOT=${CMAKE_BINARY_DIR}
  -DOPENVDB_BUILD_PYTHON_MODULE=OFF
)