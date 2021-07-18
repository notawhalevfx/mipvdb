cmake_minimum_required(VERSION 3.0.0)
project(mipvdb VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(deps)

include(CTest)
enable_testing()

add_subdirectory(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
