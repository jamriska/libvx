include(CheckIncludeFiles)
include(CheckIncludeFile)
include(CheckLibraryExists)

CHECK_INCLUDE_FILES ("malloc.h" HAVE_MALLOC_H)
CHECK_INCLUDE_FILES ("sys/param.h;sys/mount.h" HAVE_SYS_MOUNT_H)
CHECK_INCLUDE_FILES ("dlfcn.h" HAVE_DLFCN_H)
CHECK_INCLUDE_FILES ("errno.h" HAVE_ERRNO_H)
CHECK_INCLUDE_FILES ("inttypes.h" HAVE_INTTYPES_H)
CHECK_INCLUDE_FILES ("limits.h" HAVE_LIMITS_H)
CHECK_INCLUDE_FILES ("memory.h" HAVE_MEMORY_H)
CHECK_INCLUDE_FILES ("stdef.h" HAVE_STDDEF_H)
CHECK_INCLUDE_FILES ("stdint.h" HAVE_STDINT_H)
CHECK_INCLUDE_FILES ("stdlib.h" HAVE_STDLIB_H)
CHECK_INCLUDE_FILES ("stdio.h" HAVE_STDIO_H)
CHECK_INCLUDE_FILES ("string.h" HAVE_STRING_H)
CHECK_INCLUDE_FILES ("strings.h" HAVE_STRINGS_H)
CHECK_INCLUDE_FILES ("sys/stat.h" HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILES ("sys/types.h" HAVE_SYS_TYPES_H)


CHECK_LIBRARY_EXISTS("m" sin "" HAVE_LIBM)

set(srcs
	kernel.c
	source.c
	sink.c
	)

file(GLOB internal_hdrs
	"_*.h"
	)
	
	
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1)
endif()

set(misc
	config.h.cmake
	${CMAKE_SOURCE_DIR}/AUTHORS
	${CMAKE_SOURCE_DIR}/README
	${CMAKE_SOURCE_DIR}/LICENSE
	)

set_source_files_properties(${misc}
	APPEND PROPERTY
	HEADER_ONLY
	)

include_directories(
	${CMAKE_BINARY_DIR}/include
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}
	)

file(GLOB hdrs "${CMAKE_SOURCE_DIR}/include/vx/*.h")


#
# Null driver - Always available
#
set(backend_srcs
	_backends/null/null_backend.c
	_backends/null/null_backend.h
	_backends/v4l2/v4l2_backend.c
	_backends/v4l2/v4l2_backend.h
	)

set(backend_libs)

find_path(AVFOUNDATION_INCLUDE_DIR AVFoundation/AVFoundation.h)

#
# Mac OS X (10.7 upwards) and iOS
#
if(AVFOUNDATION_INCLUDE_DIR)

	message(STATUS "AVFoundation: YES")

	set(HAVE_AVFOUNDATION 1)

	list(APPEND backend_srcs
		_backends/avfoundation/avfoundation_backend.m
		_backends/avfoundation/avfoundation_backend.h
		)

	find_library(AVFOUNDATION_LIBRARY AVFoundation)
	find_library(COREMEDIA_LIBRARY CoreMedia)
	find_library(COREVIDEO_LIBRARY CoreVideo)
	find_library(FOUNDATION_LIBRARY Foundation)

	list(APPEND backend_libs ${AVFOUNDATION_LIBRARY} ${FOUNDATION_LIBRARY} ${COREMEDIA_LIBRARY} ${COREVIDEO_LIBRARY})

else()
	message(STATUS "AVFoundation: NO")
endif()


#
# QTkit is available from Mac OS X 10.5 on
#
find_path(QTKIT_INCLUDE_DIR QTKit/QTkit.h)

if(QTKIT_INCLUDE_DIR)
	set(HAVE_QTKIT 1)
	message(STATUS "QTkit: YES")
	find_library(QTkit QTKIT_LIBRARY)

	list(APPEND backend_srcs
		_backends/qtkit/qtkit_backend.m
		_backends/qtkit/qtkit_backend.h
		)

	find_library(COREMEDIA_LIBRARY CoreMedia)
	find_library(COREVIDEO_LIBRARY CoreVideo)
	find_library(QTKIT_LIBRARY QTkit)

	list(APPEND backend_libs  ${COREMEDIA_LIBRARY} ${COREVIDEO_LIBRARY} ${QTKIT_LIBRARY})

else()
	message(STATUS "QTkit: NO")

endif()

#
# Video 4 Linux is standard API on Linux
#
find_package(V4L2)
if(V4L2_FOUND)
    set(HAVE_V4L2 1)
    message(STATUS "V4L2: YES")

    list(APPEND backend_srcs
			)

    list(APPEND backend_libs ${LIBV4L2_LIBRARIES} rt)

else()
    message(STATUS "V4L2: NO")
endif()

# 
# DirectShow can be found on Windows and Windows Mobile
#
set(DSHOW_HELPER_DIR _backends/dshow/dshow_helpers)
find_package(DirectShow)
if(DIRECTSHOW_FOUND)
	message(STATUS "DirectShow: YES")
	set(HAVE_DSHOW 1)

	include_directories(${DIRECTSHOW_INCLUDE_DIRS})
   
    list(APPEND backend_srcs
            _backends/dshow/dshow_backend.cpp
            _backends/dshow/dshow_backend.h
			_backends/dshow/dshow_helpers/src/ds_capture.cpp
			_backends/dshow/dshow_helpers/src/cpropertybag.cpp
            )

    list(APPEND backend_libs ${DIRECTSHOW_LIBRARIES})

else()
   message(STATUS "DirectShow: NO")

endif() 

#
# GStreamer can be found on Linux, Mac and Windows
#
#find_package(GStreamer0.10)
#if(GSTREAMER_FOUND)
#    message(STATUS "GStreamer: YES")

#    include_directories(${GSTREAMER_INCLUDE_DIRS})
#    set(HAVE_GSTREAMER 1)
#    list(APPEND backend_srcs
#            _backends/gst010/gst010_backend.c
#            _backends/gst010/gst010_backend.h
#            )
#    list(APPEND backend_libs ${GSTREAMER_LIBRARIES})
#else()
#    message(STATUS "GStreamer: NO")
#endif()




set(VX_LIBRARY_TARGET)
option(VX_BUILD_STANDALONE_LIBRARY off)
if(VX_BUILD_STANDALONE_LIBRARY)
	set(VX_LIBRARY_TARGET SHARED)
else()
	set(VX_LIBRARY_TARGET STATIC)
endif()


configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
	${CMAKE_BINARY_DIR}/include/vx/config.h
	)

add_library(vx 
	${VX_LIBRARY_TARGET} ${srcs} ${hdrs} ${internal_hdrs} ${backend_srcs} ${misc} ${CMAKE_BINARY_DIR}/include/vx/config.h)

target_link_libraries(vx ${backend_libs})

if(VX_BUILD_STANDALONE_LIBRARY)
	set_property(TARGET vx
		APPEND PROPERTY
		COMPILE_DEFINITIONS
		VX_BUILD_LIBRARY
		)
else()
	set_property(TARGET vx
		APPEND PROPERTY
		COMPILE_DEFINITIONS
		VX_LIBRARY_STATIC
		)
endif()
