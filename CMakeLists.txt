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

# versioning
set(VX_MAJOR_VERSION 0)
set(VX_MINOR_VERSION 1)
set(VX_PATCH_VERSION 0)
set(VX_VERSION_STRING "${VX_MAJOR_VERSION}.${VX_MINOR_VERSION}.${VX_PATCH_VERSION}")

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for headers")

add_subdirectory(src)
add_subdirectory(tests)


#
# Build API docs via Doxygen
#
find_package(Doxygen)
if (DOXYGEN_FOUND)
	option(BUILD_APIDOC "Build API Documentation" OFF)
	if   (BUILD_APIDOC)

		set(PUBLIC_HEADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
		
		configure_file(
			${CMAKE_SOURCE_DIR}/etc/Doxyfile
			${CMAKE_BINARY_DIR}/.doxygen/libvx.dox
			@ONLY
			)
			
		add_custom_target("docs"
			COMMENT "Building API docs from headers"
			COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/.doxygen/libvx.dox
			DEPENDS vx
			VERBATIM
			)
		
		install(DIRECTORY 
			${CMAKE_BINARY_DIR}/doc
			DESTINATION "share/libvx"
			COMPONENT "API Documentation"
			)	
				

	endif(BUILD_APIDOC)
endif()


#
# Uninstall
#
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/etc/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
  
add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")



