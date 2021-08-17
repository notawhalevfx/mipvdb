include(ExternalProject)
include(ProcessorCount)

# blosc
ExternalProject_Add(blosc_ext
  PREFIX ${CMAKE_BINARY_DIR}/deps
  GIT_REPOSITORY git@github.com:Blosc/c-blosc.git
  GIT_TAG v1.5.0
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR} -DBUILD_STATIC=ON
)

# Get num of threads
ProcessorCount(N)
message("Found ${N} Threads")

# tbb
# TODO in future test static library
ExternalProject_Add(tbb_ext
  PREFIX ${CMAKE_BINARY_DIR}/deps
  GIT_REPOSITORY git@github.com:oneapi-src/oneTBB.git
  GIT_TAG 2018_U6
  CONFIGURE_COMMAND ""
  BUILD_IN_SOURCE ON
  BUILD_COMMAND make -j${N} tbb tbbmalloc tbbproxy
  INSTALL_COMMAND sh -c "cp -u -R ${CMAKE_BINARY_DIR}/deps/src/tbb/build/linux*/*.so* ${CMAKE_BINARY_DIR}/lib/" &&
  sh -c "cp -u -R ${CMAKE_BINARY_DIR}/deps/src/tbb/include/tbb ${CMAKE_BINARY_DIR}/include/"
)

add_library(tbb IMPORTED SHARED GLOBAL)
set_target_properties(tbb PROPERTIES
        "IMPORTED_LOCATION" "${CMAKE_BINARY_DIR}/lib/libtbb.so"
        "INTERFACE_INCLUDE_DIRECTORIES" ${CMAKE_BINARY_DIR}/include
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
ExternalProject_Add(openvdb_ext
  DEPENDS tbb_ext blosc_ext
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/deps
  GIT_REPOSITORY git@github.com:AcademySoftwareFoundation/openvdb.git
  GIT_TAG v8.1.0
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
  -DTBB_ROOT=${CMAKE_BINARY_DIR}
  -DBlosc_ROOT=${CMAKE_BINARY_DIR}
  -DOPENVDB_BUILD_PYTHON_MODULE=OFF
)

add_library(openvdb IMPORTED SHARED GLOBAL)
target_link_libraries(openvdb INTERFACE tbb)
add_dependencies(openvdb openvdb_ext)

set_target_properties(openvdb PROPERTIES
        "IMPORTED_LOCATION" "${CMAKE_BINARY_DIR}/lib64/libopenvdb.so"
        "INTERFACE_INCLUDE_DIRECTORIES" ${CMAKE_BINARY_DIR}/include
)

# boost
find_package(Boost 1.56 REQUIRED COMPONENTS filesystem program_options)

install(DIRECTORY "${CMAKE_BINARY_DIR}/lib/" DESTINATION lib
        FILES_MATCHING PATTERN "*.so*")
install(DIRECTORY "${CMAKE_BINARY_DIR}/lib64/" DESTINATION lib64
        FILES_MATCHING PATTERN "*.so*"
        PATTERN "cmake" EXCLUDE)