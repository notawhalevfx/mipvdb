configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.hpp"
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME}-bin mipvdb.cpp)

add_subdirectory(logging)
target_link_libraries(${PROJECT_NAME}-bin logging)

add_subdirectory(cli)
target_link_libraries(${PROJECT_NAME}-bin cli)

add_subdirectory(vdb_resize)
target_link_libraries(${PROJECT_NAME}-bin vdb_resize)

install(TARGETS ${PROJECT_NAME}-bin DESTINATION ${CMAKE_INSTALL_BINDIR})