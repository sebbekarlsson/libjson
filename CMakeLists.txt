cmake_minimum_required(VERSION 3.20)


project(json)

file(GLOB PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB json_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)


add_executable(json_e ${json_sources})


list(FILTER json_sources EXCLUDE REGEX "main.c")

add_library(json SHARED ${json_sources})
add_library(json_static STATIC ${json_sources})

target_compile_options(json PUBLIC -fPIC)
target_compile_options(json_static PUBLIC -fPIC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(
  json
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_include_directories(
  json_static
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_include_directories(
  json_e
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


include(FetchContent)

FetchContent_Declare(
  hashmap_static
  GIT_REPOSITORY https://github.com/sebbekarlsson/hashmap.git
  )

if (NOT TARGET hashmap_static)
  FetchContent_MakeAvailable(hashmap_static)
endif()


target_link_libraries(json PRIVATE hashmap_static)
target_link_libraries(json_static PRIVATE hashmap_static)
target_link_libraries(json_e PRIVATE hashmap_static)
