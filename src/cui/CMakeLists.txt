set(SRCS 
    cui.cpp
)

add_library(cui STATIC ${SRCS})

target_link_libraries(cui Boost::boost Boost::program_options Boost::filesystem)