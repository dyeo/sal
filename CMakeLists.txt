cmake_minimum_required(VERSION 3.1...3.16)

project (sal VERSION 0.1 LANGUAGES C)

file(GLOB_RECURSE SOURCE "./src/*.c")

add_executable(sal ${SOURCE})

target_include_directories(sal PUBLIC "./include")

add_definitions(-DLOG_USE_COLOR)