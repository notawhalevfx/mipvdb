include(ExternalProject)
include(GNUInstallDirs)
include(ProcessorCount)

# Make some extra directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib64)

# blosc
ExternalProject_Add(blosc_ext
  PREFIX ${CMAKE_BINARY_DIR}/deps
  GIT_REPOSITORY https://github.com/Blosc/c-blosc.git
  GIT_TAG v1.17.0
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR} -DBUILD_STATIC=ON
)

# Get num of threads
ProcessorCount(N)
message("Found ${N} Threads")

# tbb
# TODO in future test static library
ExternalProject_Add(tbb_ext
  PREFIX ${CMAKE_BINARY_DIR}/deps
  GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
  GIT_TAG 2020_U3
  BUILD_BYPRODUCTS "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/libtbb.so"
  CONFIGURE_COMMAND ""
  BUILD_IN_SOURCE ON
  BUILD_COMMAND make -j${N} tbb tbbmalloc tbbproxy
  INSTALL_COMMAND sh -c "cp -u -R ${CMAKE_BINARY_DIR}/deps/src/tbb_ext/build/linux*/*.so* ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/" &&
  sh -c "cp -u -R ${CMAKE_BINARY_DIR}/deps/src/tbb_ext/include/tbb ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}"
)

add_library(tbb IMPORTED SHARED GLOBAL)
set_target_properties(tbb PROPERTIES
        "IMPORTED_LOCATION" "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/libtbb.so"
        "INTERFACE_INCLUDE_DIRECTORIES" ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}
)

# jemalloc
ExternalProject_Add(jemalloc_ext
  PREFIX ${CMAKE_BINARY_DIR}/deps
  URL https://github.com/jemalloc/jemalloc/releases/download/5.2.1/jemalloc-5.2.1.tar.bz2
  CONFIGURE_COMMAND ./configure --prefix=${CMAKE_BINARY_DIR}
  BUILD_IN_SOURCE ON
  BUILD_COMMAND make -j${N} build_lib
  INSTALL_COMMAND make install_lib
)

# OpenVDB
ExternalProject_Add(openvdb_ext
  DEPENDS tbb_ext blosc_ext jemalloc_ext
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/deps
  GIT_REPOSITORY https://github.com/AcademySoftwareFoundation/openvdb.git
  GIT_TAG v9.0.0
  BUILD_BYPRODUCTS "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/libopenvdb.so"
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
  -DTBB_ROOT=${CMAKE_BINARY_DIR}
  -DBlosc_ROOT=${CMAKE_BINARY_DIR}
  -DOPENVDB_BUILD_PYTHON_MODULE=OFF
  -DOPENVDB_BUILD_BINARIES=OFF
)

add_library(openvdb IMPORTED SHARED GLOBAL)
target_link_libraries(openvdb INTERFACE tbb)
add_dependencies(openvdb openvdb_ext)

set_target_properties(openvdb PROPERTIES
        "IMPORTED_LOCATION" "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/libopenvdb.so"
        "INTERFACE_INCLUDE_DIRECTORIES" ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}
)

# boost
find_package(Boost 1.56 REQUIRED COMPONENTS filesystem program_options)

# install(DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/" DESTINATION ${CMAKE_INSTALL_LIBDIR}
#         FILES_MATCHING PATTERN "*.so*")
install(DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/" DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FILES_MATCHING PATTERN "*.so*"
        PATTERN "cmake" EXCLUDE)