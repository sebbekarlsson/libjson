cmake_minimum_required(VERSION 3.20)


project(json)

file(GLOB sources src/*)

add_library(json SHARED ${sources})

include_directories(src/include/*)
