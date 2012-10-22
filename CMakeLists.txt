cmake_minimum_required(VERSION 2.8)

project(videoextractor)


include(CheckIncludeFiles)
include(CheckIncludeFile)
#include(CheckPrototypeExists)

configure_file(
	src/vx/config.h.cmake
	${CMAKE_BINARY_DIR}/include/vx/config.h
	)

add_subdirectory(src)
add_subdirectory(tests)
