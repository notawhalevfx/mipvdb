configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.hpp"
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} mipvdb.cpp)
target_link_libraries(${PROJECT_NAME} openvdb)

add_subdirectory(logging)
target_link_libraries(${PROJECT_NAME} logging)

add_subdirectory(cui)
target_link_libraries(${PROJECT_NAME} cui)

add_subdirectory(vdb_resize)
target_link_libraries(${PROJECT_NAME} vdb_resize)