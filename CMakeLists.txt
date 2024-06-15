cmake_minimum_required(VERSION 3.20)
project(libTMK)
add_library(libTMK STATIC "${CMAKE_CURRENT_LIST_DIR}/src/TMK.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/TMK.h")
target_include_directories(libTMK PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src")
