cmake_minimum_required(VERSION 3.1...3.16)

project (sal VERSION 0.1 LANGUAGES C)

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(sal ${SOURCE})

target_include_directories(sal PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
