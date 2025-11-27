
add_library(cute_png STATIC cute_png.h cute_png.cpp)
target_include_directories(cute_png PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
