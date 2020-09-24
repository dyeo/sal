cmake_minimum_required(VERSION 3.1...3.16)

project (sal VERSION 0.1 LANGUAGES C)

file(GLOB parser_SRC
    "./parser/*.h"
    "./parser/*.c"
)

add_executable(sal main.c ${parser_SRC})

target_include_directories(sal PUBLIC "./include")

