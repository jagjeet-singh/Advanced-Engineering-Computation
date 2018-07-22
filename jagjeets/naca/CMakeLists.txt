add_library(naca4digit naca4digit.h naca4digit.cpp)
target_link_libraries(naca4digit ysclass utility)
target_include_directories(naca4digit PUBLIC .)
