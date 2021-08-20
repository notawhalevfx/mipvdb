get_filename_component(LIBRARY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(SRCS 
    logging.cpp
    memory.cpp
)

add_library(${LIBRARY_NAME} STATIC ${SRCS})

# target_link_libraries(${LIBRARY_NAME})