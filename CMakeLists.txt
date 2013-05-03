#
# CMake build system for libVX
#

cmake_minimum_required(VERSION 2.8)

project(videoextractor)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/etc/cmake ${CMAKE_MODULE_PATH})

# deal with Windows
set(LIB_OUTPUT_PATH lib)
if (WIN32)
	set(LIB_OUTPUT_PATH bin)
endif()

# some general setup
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Executables go here")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Libraries go here")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIB_OUTPUT_PATH} CACHE PATH "Standalone DLLs go here")

# prevent direct installation into the host machine
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/staging CACHE PATH "Installation directory" FORCE)
set(VX_MAJOR_VERSION 0)
set(VX_MINOR_VERSION 1)
set(VX_PATCH_VERSION 0)
set(VX_VERSION ${VX_MAJOR_VERSION}.${VX_MINOR_VERSION}.${VX_PATCH_VERSION})

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for headers")


include(CheckIncludeFiles)
include(CheckIncludeFile)



add_subdirectory(src)
add_subdirectory(tests)


