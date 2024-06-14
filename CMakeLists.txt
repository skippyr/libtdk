cmake_minimum_required(VERSION 3.20)
project(libtmk)
add_library(libtmk STATIC "${CMAKE_CURRENT_LIST_DIR}/src/tmk.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/tmk.hpp")
target_include_directories(libtmk PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src")
