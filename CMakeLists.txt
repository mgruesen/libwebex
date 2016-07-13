cmake_minimum_required(VERSION 2.8.12)
enable_testing()
# Project Web Extraction
project(webex CXX)

# Compiler check
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8")
    message(FATAL_ERROR "Requires GCC version 4.8 or greater")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -stdlib=libc++")
endif()

# Allow includes to find from this dir.
include_directories(.)

add_subdirectory(libwebex)
add_subdirectory(test)
